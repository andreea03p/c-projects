/*
The 24-bit per pixel (24bpp) format supports 16,777,216 distinct colors and stores 1 pixel value per 3 bytes.
Each pixel value defines the red, green and blue samples of the pixel (8.8.8.0.0 in RGBAX notation).
Specifically, in the order: blue, green and red (8 bits per each sample)
*/
// convert an 24-bit color bmp picture into a black&white (greyscale) one

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#pragma pack(push, 1) // set alignment to 1 byte boundary <=> avoid padding between structure members

typedef struct bmp_header
{
    char signature[2];
    unsigned int bmpFileSize;
    int unused;
    unsigned int offset;

} bmp_header;

typedef struct dib_header
{
    int headerBytes;
    int width;
    int height;
    short int colorPlanes;
    short int bpp;
    int compression;
    int imageSize;
    int ppmOX;
    int ppmOY;
    int colorsUsed;
    int impColors;

} dib_header;

typedef struct Pixel
{
    unsigned char blue;
    unsigned char green;
    unsigned char red;

} Pixel;


void convert(FILE *f, bmp_header* bmpH, dib_header* dibH, char *outf)
{
    FILE *fileOut = fopen(outf, "wb");
    if (fileOut == NULL)
    {
        perror("error: can't open OUTPUT file\n");
        exit(EXIT_FAILURE);
    }

    fwrite(bmpH, sizeof(bmp_header), 1, fileOut);
    fwrite(dibH, sizeof(dib_header), 1, fileOut);


    Pixel** newImage = malloc(dibH->height * sizeof(Pixel*));
    if(newImage == NULL)
    {
        perror("error allocating new image\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < dibH->height; ++i) 
    {
        newImage[i] = malloc(dibH->width * sizeof(Pixel));
        if (newImage[i] == NULL)
        {
            perror("error allocating row\n");
            exit(EXIT_FAILURE);
        }

        for (int y = 0; y < dibH->width; ++y) 
        {
            fread(&newImage[i][y], sizeof(Pixel), 1, f);
            unsigned char grayscale = (unsigned char)(0.3 * newImage[i][y].red + 0.59 * newImage[i][y].green + 0.11 * newImage[i][y].blue);
            newImage[i][y].blue = grayscale;
            newImage[i][y].green = grayscale;
            newImage[i][y].red = grayscale;
        }
    }

    for (int i = 0; i < dibH->height; ++i) 
    {
        for (int y = 0; y < dibH->width; ++y) 
        {
            fwrite(&newImage[i][y], sizeof(Pixel), 1, fileOut);
        }
    }

    for (int i = 0; i < dibH->height; ++i) 
    {
        free(newImage[i]);
    }
    free(newImage);

    fclose(fileOut);
}


void readHeader(FILE *f, bmp_header* imgHeader)
{
    printf("size of BMP HEADER: %ld\n", sizeof(bmp_header));

    fread(imgHeader, sizeof(bmp_header), 1, f);
    if (imgHeader->signature[0] != 'B' || imgHeader->signature[1] != 'M')
    {
        perror("error: the file is not BMP format\n");
        exit(EXIT_FAILURE);
    }

    printf("bmp file size: %d\noffset: %u\n", imgHeader->bmpFileSize, imgHeader->offset);
}


void readImgInfo(FILE *f, dib_header* dibHeader)
{
    printf("size of DIB HEADER: %ld\n", sizeof(dib_header));

    fread(dibHeader, sizeof(dib_header), 1, f);
    printf("Offset of width: %lu\n", sizeof(bmp_header) + offsetof(dib_header, width));
    printf("Offset of height: %lu\n", sizeof(bmp_header) + offsetof(dib_header, height));
    printf("Offset of bits per pixel: %lu\n", sizeof(bmp_header) + offsetof(dib_header, bpp));

    int width, height, bitDepth, compress, colorP;

    width = dibHeader->width;
    height = dibHeader->height;
    bitDepth = dibHeader->bpp;
    compress = dibHeader->compression;
    colorP = dibHeader->colorPlanes;

    printf("image width: %d\nimage height: %d\nimage bits per pixel: %d\ncompression: %d\ncolorPlanes: %d\n", width, height, bitDepth, compress, colorP);
    printf("\n");
}


int main()
{
    FILE *fileIn = fopen("porsche.bmp", "rb");
    if (fileIn == NULL)
    {
        perror("error: can't open INPUT file\n");
        exit(EXIT_FAILURE);
    }

    bmp_header* bmpH = malloc(sizeof(bmp_header));
    if (bmpH == NULL)
    {
        perror("error allocating memory\n");
        exit(EXIT_FAILURE);
    }
    readHeader(fileIn, bmpH);

    printf("\n");

    dib_header* dibH = malloc(sizeof(dib_header));
    if (dibH == NULL)
    {
        perror("error allocating memory\n");
        exit(EXIT_FAILURE);
    };
    readImgInfo(fileIn, dibH);

    printf("%d\n", dibH->bpp);

    if (dibH->bpp != 24)
    {
        printf("expected 24 bit format :(\n");
        exit(EXIT_FAILURE);
    }
    else
    {
        convert(fileIn, bmpH, dibH, "output.bmp");
    }

    free(bmpH);
    free(dibH);
    fclose(fileIn);

    return 0;
}
