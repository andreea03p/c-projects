#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char **build_table(char **table)
{
    table = malloc(11 * sizeof(char *));
    for (int i = 0; i < 11; i++)
    {
        table[i] = malloc(11 * sizeof(char));
    }

    char k = '1';

    for (int i = 0; i < 11; i++)
    {
        for (int j = 0; j < 11; j++)
        {
            if (i == 3 || i == 7)
            {
                table[i][j] = '-';
            }
            else if (j == 3 || j == 7)
            {
                table[i][j] = '|';
            }
            else if ((i == 1 || i == 5 || i == 9) && (j == 1 || j == 5 || j == 9))
            {
                table[i][j] = k;
                k++;
            }
            else
            {
                table[i][j] = ' ';
            }
        }
    }

    return table;
}

void print_table(char **table)
{
    for (int i = 0; i < 11; i++)
    {
        for (int j = 0; j < 11; j++)
        {
            printf("%c", table[i][j]);
        }
        printf("\n");
    }
}

void free_table(char **table)
{
    for(int i = 0; i < 11; i++)
    {
        free(table[i]);
    }
    free(table);
}

int check_winner(char **table)
{
    if (table[1][1] == table[1][5] && table[1][5] == table[1][9])
    {
        return 1;
    }
    else if (table[5][1] == table[5][5] && table[5][5] == table[5][9])
    {
        return 1;
    }
    else if (table[9][1] == table[9][5] && table[9][5] == table[9][9])
    {
        return 1;
    }
    else if (table[1][1] == table[5][1] && table[5][1] == table[9][1])
    {
        return 1;
    }
    else if (table[1][5] == table[5][5] && table[5][5] == table[9][5])
    {
        return 1;
    }
    else if (table[1][9] == table[5][9] && table[5][9] == table[9][9])
    {
        return 1;
    }
    else if (table[1][1] == table[5][5] && table[5][5] == table[9][9])
    {
        return 1;
    }
    else if (table[1][9] == table[5][5] && table[5][5] == table[9][1])
    {
        return 1;
    }

    return 0;
}

char **modify_table(char **table, int i, int j, char a)
{
    table[i][j] = a;

    return table;
}

void play(char **table)
{
    printf("player 1: X\nplayer 2: 0\n");
    print_table(table);

    int a, b;
    int v[10] = {0};
    int count = 0;

    while (check_winner(table) != 1 && count < 9)
    {
        do
        {
            printf("player 1 pick: ");
            scanf("%d", &a);
        } while (a < 1 || a > 9 || v[a] > 0);

        count++;
        v[a]++;

        if (v[a] == 1)
        {
            switch (a)
            {
            case 1:
            {
                table = modify_table(table, 1, 1, 'X');
                print_table(table);
                if(check_winner(table))
                {
                    printf("the winner is : PLAYER 1\n");
                    free_table(table);
                    exit(-1);
                }
                break;
            }
            case 2:
            {
                table = modify_table(table, 1, 5, 'X');
                print_table(table);
                if(check_winner(table))
                {
                    printf("the winner is : PLAYER 1\n");
                    free_table(table);
                    exit(-1);
                }
                break;
            }
            case 3:
            {
                table = modify_table(table, 1, 9, 'X');
                print_table(table);
                if(check_winner(table))
                {
                    printf("the winner is : PLAYER 1\n");
                    free_table(table);
                    exit(-1);
                }
                break;
            }
            case 4:
            {
                table = modify_table(table, 5, 1, 'X');
                print_table(table);
                if(check_winner(table))
                {
                    printf("the winner is : PLAYER 1\n");
                    free_table(table);
                    exit(-1);
                }
                break;
            }
            case 5:
            {
                table = modify_table(table, 5, 5, 'X');
                print_table(table);
                if(check_winner(table))
                {
                    printf("the winner is : PLAYER 1\n");
                    free_table(table);
                    exit(-1);
                }
                break;
            }
            case 6:
            {
                table = modify_table(table, 5, 9, 'X');
                print_table(table);
                if(check_winner(table))
                {
                    printf("the winner is : PLAYER 1\n");
                    free_table(table);
                    exit(-1);
                }
                break;
            }
            case 7:
            {
                table = modify_table(table, 9, 1, 'X');
                print_table(table);
                if(check_winner(table))
                {
                    printf("the winner is : PLAYER 1\n");
                    free_table(table);
                    exit(-1);
                }
                break;
            }
            case 8:
            {
                table = modify_table(table, 9, 5, 'X');
                print_table(table);
                if(check_winner(table))
                {
                    printf("the winner is : PLAYER 1\n");
                    free_table(table);
                    exit(-1);
                }
                break;
            }
            case 9:
            {
                table = modify_table(table, 9, 9, 'X');
                if(check_winner(table))
                {
                    printf("the winner is : PLAYER 1\n");
                    free_table(table);
                    exit(-1);
                }
                break;
            }
            }
        }
        else
        {
            printf("invalid pick\n");
        }

        if(count == 9)
        {
            printf("IT'S A TIE! :/\nplay again!\n");
            free_table(table);
            return;
        }

        do
        {
            printf("player 2 pick: ");
            scanf("%d", &b);
        } while (b < 1 || b > 9 || v[b] > 1);

        count++;
        v[b]++;

        if (v[b] == 1)
        {
            switch (b)
            {
            case 1:
            {
                table = modify_table(table, 1, 1, '0');
                print_table(table);
                if(check_winner(table))
                {
                    printf("the winner is : PLAYER 2\n");
                    free_table(table);
                    exit(-1);
                }
                break;
            }
            case 2:
            {
                table = modify_table(table, 1, 5, '0');
                print_table(table);
                if(check_winner(table))
                {
                    printf("the winner is : PLAYER 2\n");
                    free_table(table);
                    exit(-1);
                }
                break;
            }
            case 3:
            {
                table = modify_table(table, 1, 9, '0');
                print_table(table);
                if(check_winner(table))
                {
                    printf("the winner is : PLAYER 2\n");
                    free_table(table);
                    exit(-1);
                }
                break;
            }
            case 4:
            {
                table = modify_table(table, 5, 1, '0');
                print_table(table);
                if(check_winner(table))
                {
                    printf("the winner is : PLAYER 2\n");
                    free_table(table);
                    exit(-1);
                }
                break;
            }
            case 5:
            {
                table = modify_table(table, 5, 5, '0');
                print_table(table);
                if(check_winner(table))
                {
                    printf("the winner is : PLAYER 2\n");
                    free_table(table);
                    exit(-1);
                }
                break;
            }
            case 6:
            {
                table = modify_table(table, 5, 9, '0');
                print_table(table);
                if(check_winner(table))
                {
                    printf("the winner is : PLAYER 2\n");
                    free_table(table);
                    exit(-1);
                }
                break;
            }
            case 7:
            {
                table = modify_table(table, 9, 1, '0');
                print_table(table);
                if(check_winner(table))
                {
                    printf("the winner is : PLAYER 2\n");
                    free_table(table);
                    exit(-1);
                }
                break;
            }
            case 8:
            {
                table = modify_table(table, 9, 5, '0');
                print_table(table);
                if(check_winner(table))
                {
                    printf("the winner is : PLAYER 2\n");
                    free_table(table);
                    exit(-1);
                }
                break;
            }
            case 9:
            {
                table = modify_table(table, 9, 9, '0');
                print_table(table);
                if(check_winner(table))
                {
                    printf("the winner is : PLAYER 2\n");
                    free_table(table);
                    exit(-1);
                }
                break;
            }
            }
        }
        else
        {
            printf("invalid pick\n");
        }
    }

    printf("IT'S A TIE! :/\n play again!\n");
    free_table(table);
    return;
}

int main()
{
    char **table = NULL;
    table = build_table(table);

    play(table);

    return 0;
}
