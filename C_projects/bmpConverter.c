/*
The 24-bit per pixel (24bpp) format supports 16,777,216 distinct colors and stores 1 pixel value per 3 bytes.
Each pixel value defines the red, green and blue samples of the pixel (8.8.8.0.0 in RGBAX notation).
Specifically, in the order: blue, green and red (8 bits per each sample)
*/

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

typedef struct PixelsData
{
    unsigned char blue;
    unsigned char green;
    unsigned char red;

} PixelsData;


void convert(FILE *f, bmp_header* bmpH, dib_header* dibH, char *outf)
{
    FILE *fileOut = fopen(outf, "wb");
    if (fileOut == NULL)
    {
        perror("error: can't open OUTPUT file\n");
        exit(EXIT_FAILURE);
    }

    if( fwrite(bmpH, sizeof(bmp_header), 1, fileOut) != 1)
    {
        perror("error writing bmp header in output file\n");
        exit(EXIT_FAILURE);
    }
    if (bmpH->signature[0] != 'B' || bmpH->signature[1] != 'M')
    {
        perror("error: the output file is not BMP format\n");
        exit(EXIT_FAILURE);
    }

    if(fwrite(dibH, sizeof(dib_header), 1, fileOut) != 1)
    {
        perror("error writing dib header in output file\n");
        exit(EXIT_FAILURE);
    }

    PixelsData** newImage = malloc(dibH->height * sizeof(PixelsData*));
    if(newImage == NULL)
    {
        perror("error allocating new image\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < dibH->height; ++i) 
    {
        newImage[i] = malloc(dibH->width * sizeof(PixelsData));
        if (newImage[i] == NULL)
        {
            perror("error allocating row\n");
            exit(EXIT_FAILURE);
        }

        for (int j = 0; j < dibH->width; ++j) 
        {
            if(fread(&newImage[i][j], sizeof(PixelsData), 1, f) != 1)
            {
                perror("error reading matrix item from output file\n");
                exit(EXIT_FAILURE);
            }
            unsigned char grayscale = (unsigned char)(0.3 * newImage[i][j].red + 0.6 * newImage[i][j].green + 0.1 * newImage[i][j].blue);
            newImage[i][j].blue = grayscale;
            newImage[i][j].green = grayscale;
            newImage[i][j].red = grayscale;
        }
    }

    for (int i = 0; i < dibH->height; ++i) 
    {
        for (int j = 0; j < dibH->width; ++j)
        {
            if(fwrite(&newImage[i][j], sizeof(PixelsData), 1, fileOut) != 1)
            {
                perror("error writing matrix item in output file\n");
                exit(EXIT_FAILURE);
            }
        }
    }

    for (int i = 0; i < dibH->height; ++i) 
    {
        free(newImage[i]);
    }
    free(newImage);

    fclose(fileOut);

    printf("\nSuccessful Conversion! \n");
}


void readHeader(FILE *f, bmp_header* imgHeader)
{
    printf("size of BMP HEADER: %ld\n", sizeof(bmp_header));

    if(fread(imgHeader, sizeof(bmp_header), 1, f) != 1)
    {
        perror("error reading bmp header from input file\n");
        exit(EXIT_FAILURE);
    }
    if(imgHeader->signature[0] != 'B' || imgHeader->signature[1] != 'M')
    {
        perror("error: the input file is not BMP format\n");
        exit(EXIT_FAILURE);
    }

    printf("bmp file size: %d\noffset: %u\n", imgHeader->bmpFileSize, imgHeader->offset);
}


void readImgInfo(FILE *f, dib_header* dibHeader)
{
    printf("size of DIB HEADER: %ld\n", sizeof(dib_header));

    if(fread(dibHeader, sizeof(dib_header), 1, f) != 1)
    {
        perror("error reading dib header info from input file\n");
        exit(EXIT_FAILURE);
    }
    printf("Offset of width: %lu\n", sizeof(bmp_header) + offsetof(dib_header, width));
    printf("Offset of height: %lu\n", sizeof(bmp_header) + offsetof(dib_header, height));
    printf("Offset of bits per pixel: %lu\n", sizeof(bmp_header) + offsetof(dib_header, bpp));

    int width, height, bitDepth, compress, colorP;

    width = dibHeader->width;
    height = dibHeader->height;
    bitDepth = dibHeader->bpp;
    compress = dibHeader->compression;
    colorP = dibHeader->colorPlanes;

    printf("image width: %d\nimage height: %d\nimage bits per pixel: %d\ncompression: %d\ncolorPlanes: %d", width, height, bitDepth, compress, colorP);
    printf("\n");
}


int main()
{
    FILE *fileIn = fopen("book.bmp", "rb");
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
    }
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
        free(bmpH);
        free(dibH);
    }
    
    fclose(fileIn);

    return 0;
}
