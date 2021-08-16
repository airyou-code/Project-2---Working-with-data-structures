#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct table 
{
    char ***words;
    int **inf_s;
    int *inf_c;

}tab;

typedef struct variables 
{
    char *delim;
    int cnt_row;
    int cnt_cell;
    int cnt_words;
    int max_size;
}var;

typedef struct variables_n 
{
    int free_r;
    int *free_c;
    int **free_s;
}free_var;

// void preparation_free (tab *w, var *a, free_var *b);
int delim (int argc, char *argv[], var *a);
char check_delim (var *a, char symbol);
void read_arg( tab *w, var *a, free_var *b, char *argv[], int num); //ĐžĐąŃĐľĐˇĐ°ŃŃ ĐşĐžĐťĐ¸ŃĐľŃŃĐ˛Đž ĐżĐľŃĐľĐźĐľĐ˝Đ˝ŃŃ
void parsing_arg ( tab *w, var *a, free_var *b,char **my_arg, int my_argc);
void expand (tab *w, var *a, free_var *b, int R1, int C1, int R2, int C2);
void selection (var *a, int *i, int *row, int *j, int *R1, int *C1, int *R2, int *C2, int *max);
void set ( tab *w, var *a, free_var *b, char **my_arg, int n, int R1, int C1, int R2, int C2);
void icol (tab *w, var *a, free_var *b, int C1, int s);
void acol (tab *w, var *a, free_var *b, int C1, int s);
void dcol (tab *w, var *a, free_var *b,int C1);
void irow (tab *w, var *a, free_var *b, int R1);
void arow (tab *w, var *a, free_var *b, int R1);
void drow (tab *w, var *a, free_var *b, int R1);
void clear (tab *w, var *a, int R1, int C1, int R2, int C2);
int read_words_from_file(char *filename, tab *w, free_var *b, var *a);
void realloc_row (tab *w, free_var *b, int cnt_row, int sw);
void realloc_cell (tab *w, free_var *b,int cnt_row, int cnt_cell, int sw);
void malloc_cell (tab *w, free_var *b, int cnt_row, int sw);
void realloc_words (tab *w, free_var *b, int cnt_row, int cnt_cell, int cnt_words);
void malloc_words (tab *w, free_var *b, int cnt_row, int cnt_cell);
void write_to_array (tab *w, free_var *b, var *a, FILE *f);
int word_to_array (tab *w, var *a, free_var *b, FILE *f, char symbol);
void print_table (tab *w, var *a, char *filename);
void trimming_table (tab *w, var *a, free_var *b, int n);
void row_plus (tab *w, var *a, free_var *b, int n);

int main(int argc, char *argv[]) 
{
    tab w = {NULL, 0, 0};
    var a = {NULL, 0, 0, 0, 0};
    free_var b = {0, 0, 0};

    int text = 4;
    if (delim(argc, argv, &a) == 3)
    {
        return 0;
    }

    if (!delim(argc, argv, &a))
    {
        text = 2;
    }
    
    if (!read_words_from_file(argv[text], &w, &b, &a))
    {
        fprintf(stderr, "== Keep the file ==\n");
        return 1;
    }

    trimming_table (&w, &a, &b, 0);

    read_arg( &w, &a, &b, argv, text-1);
    // preparation_free (&w, &a, &b);
    print_table (&w, &a, argv[text]);
    return 0;
}

// void preparation_free (tab *w, var *a, free_var *b) // TO DO
// {
    // // int s = 0;
    // printf("=table use realloc=\n\n");
    // for (int i = 0; i < b->free_r; i++)
    // {
    //     printf("   ");
    //     for (int j = 0; j < b->free_c[i]; j++)
    //     {

    //        printf(" %d;%d", b->free_s[i][j], b->free_c[i]);
    //     }
    //     printf("\n");
    // }
    // printf("\n");


    // int s = 0;
    // // printf("=table use realloc=\n\n");
    // for (int i = 0; i < b->free_r; i++)
    // {
    //     // printf("   ");
    //     for (int j = 0; j < b->free_c[i]; j++)
    //     {
    //        for (int k = b->free_s[i][j]; k > 0; k--)
    //        {
    //         //    free(w->words[i][j][k]);
    //         //    w->words[i][j][k-1] = '\0';
    //        }
    //         // free(w->words[i][j]);
    //     }
    //     // free(w->words[i]);
    //     // printf("\n");
    // }
    // // printf("\n");
    // // printf("cell %i\n", b->free_c[1]);
    // // printf("row %i\n", b->free_r);
    // // printf("symbol %i\n", b->free_s[0][0]);
    // // printf("%c\n\n", w->words[0][0][5]);
// }

// 
int delim (int argc, char *argv[], var *a)
{
    a->delim = malloc(sizeof(char));
    a->delim[0] = ' ';
    if (argc < 1)
    {
        return 3;
    }
    if (strcmp(argv[1], "-d") == 0)
    {
        for (int j = 1; argv[2][j-1] != '\0'; j++)
        {
            a->delim = realloc(a->delim, sizeof(char) * j);
            a->delim[j-1] = argv[2][j-1];
        }
    }
    else
    {
        a->delim[0] = ' ';
        return 0;
    }
    return 1;
}

// replacing delim with the first delim
char check_delim (var *a, char symbol)
{
    for ( int i = 0; a->delim[i] != '\0'; i++)
    {
        if (symbol == a->delim[i])
        {
            symbol = a->delim[0];
            return symbol;
        }
    }
    return symbol;
}

// writing orders to "my_arg"
void read_arg( tab *w, var *a, free_var *b, char *argv[], int num)
{
    char **my_arg;
    int my_argc = 0;
    int j = 0;
    int k = 0;

    for (int i = 0; j < (int) strlen(argv[num]); i++ )
    {
        my_argc++;
        if (i == 0)
        {
            my_arg = malloc(sizeof(char));
        }
        else
        {
            my_arg = realloc(my_arg, sizeof(char*) * (i+1));
        }

        k = 0;

        while (1)
        {
            if(k == 0)
            {
                my_arg[i] = malloc(sizeof(char**));
            }
            else
            {
                my_arg[i] = realloc(my_arg[i], sizeof(char**) * (k+1));
            }

            j++;
            if(argv[num][j-1] == 59) break;
            if(argv[num][j-1] == 32) break;
            if(argv[num][j-1] == '\0') break;

            k++;
            my_arg[i][k-1] = argv[num][j-1];
        }
        my_arg[i][k] = '\0';
    }

   parsing_arg ( w, a, b, my_arg, my_argc);
}

void parsing_arg ( tab *w, var *a, free_var *b, char **my_arg, int my_argc)
{
    char var_R1[100];
    char var_C1[100];
    char var_R2[100];
    char var_C2[100];
    int R1 = -1;
    int C1 = -1;
    int R2 = -1;
    int C2 = -1;
    var_R1[0] = '\0';
    var_C1[0] = '\0';
    var_R2[0] = '\0';
    var_C2[0] = '\0';
    int counter1 = 0;
    int counter2 = 0;
    for (int i = 0; i < my_argc; i++)
    {
        if (sscanf(my_arg[i], "[%[^,],%[^,],%[^,],%[^]]]", var_R1, var_C1, var_R2, var_C2) == 4)
        {
            if(var_R1[0] == '_')
            {
               var_R1[0] = '0';
            }

            if(var_C1[0] == '_')
            {
               var_C1[0] = '0';
            }

            if(var_R2[0] == '_')
            {
                var_R2[0] = '0';
            }

            if(var_C2[0] == '_')
            {
               var_C2[0] = '0';
            }

            R1 = atoi(var_R1);
            C1 = atoi(var_C1);
            R2 = atoi(var_R2);
            C2 = atoi(var_C2);
        }
        else
            if (sscanf(my_arg[i], "[%[^,],%[^]]]", var_R1, var_C1) == 2)
            {
                if(var_R1[0] == '_')
                {
                    var_R1[0] = '0';
                }

                if(var_C1[0] == '_')
                {
                    var_C1[0] = '0';
                }

                R1 = atoi(var_R1);
                C1 = atoi(var_C1);
                R2 = -1;
                C2 = -1;
            }

        expand (w, a, b, R1, C1, R2, C2);

        if (strcmp(my_arg[i], "set") == 0)
        {
            set ( w, a, b, my_arg, i+1, R1, C1, R2, C2);
        }

        if (strcmp(my_arg[i], "icol") == 0)
        {
            counter1++;
            icol ( w, a, b, C1, counter1);
        }

        if (strcmp(my_arg[i], "acol") == 0)
        {
            counter2++;
            acol ( w, a, b, C1, counter2);
        }

        if (strcmp(my_arg[i], "irow") == 0)
        {
            irow ( w, a, b, R1);
        }  

        if (strcmp(my_arg[i], "arow") == 0)
        {
            arow ( w, a, b, R1);
        }

        if (strcmp(my_arg[i], "drow") == 0)
        {
            drow ( w, a, b, R1);
        }

        if (strcmp(my_arg[i], "clear") == 0)
        {
            clear ( w, a, R1, C1, R2, C2);
        } 

        if (strcmp(my_arg[i], "dcol") == 0)
        {
            dcol ( w, a, b, C1);
        } 
              
    }
}

void expand (tab *w, var *a, free_var *b, int R1, int C1, int R2, int C2)
{
    int i = 0;
    int j = 0;
    int row = 0;
    int max = 0;
    int qt1 = a->cnt_row;
    int qt2 = a->max_size;
    int counter2 = 1;
    selection (a, &i, &row, &j, &R1, &C1, &R2, &C2, &max);

    for (;qt1 < R1; qt1++)
    {
        arow ( w, a, b, a->max_size);
    }

    for (;qt2 < C1; qt2++)
    {
        counter2++;
        acol ( w, a, b, a->cnt_row, counter2);
    }
}

void dcol (tab *w, var *a, free_var *b, int C1)
{
    int x = 0;
    x = C1;
    trimming_table (w, a, b, 1);
    for (int i = 0; i < a->cnt_row; i++)
    {
        for (int k = 0; k < a->max_size-x; k++)
        strcpy(w->words[i][k+x-1], w->words[i][k+x]);
    }
    a->max_size = a->max_size - 2;
}

void drow (tab *w, var *a, free_var *b, int R1)
{
    row_plus (w, a, b, 1);
    a->cnt_row = a->cnt_row + 1;
    int x = 0;
    x = R1;
    for (int i = 0; i < a->cnt_row-x; i++)
    {
        for (int j = 0; j < a->max_size; j++)
        {
            strcpy(w->words[i+x-1][j], w->words[i+x][j]);
        }
    }

    a->cnt_row = a->cnt_row - 2;

}

void irow (tab *w, var *a, free_var *b, int R1)
{
    row_plus (w, a, b, 1);
    a->cnt_row = a->cnt_row + 1;            //ĐżŃĐžĐąĐťĐľĐźĐ° ŃĐ¸ŃŃĐľĐźŃ
    for (int i = a->cnt_row-2; i >= R1-1; i--)
    {
        for (int j = 0; j < a->max_size; j++)
        {
            strcpy(w->words[i+1][j], w->words[i][j]);
        }
    }

    for (int j = 0; j < a->max_size; j++)
    {
        w->words[R1-1][j][0] = '\0';
    }
}

void arow (tab *w, var *a, free_var *b, int R1)
{
    row_plus (w, a, b, 1);
    a->cnt_row = a->cnt_row + 1;                //ĐżŃĐžĐąĐťĐľĐźĐ° ŃĐ¸ŃŃĐľĐźŃ
    for (int i = a->cnt_row-2; i >= R1-1; i--)
    {
        for (int j = 0; j < a->max_size; j++)
        {
            strcpy(w->words[i+1][j], w->words[i][j]);
        }
    }

    for (int j = 0; j < a->max_size; j++)
    {
        w->words[R1][j][0] = '\0';
    }
}

void clear (tab *w, var *a, int R1, int C1, int R2, int C2)
{
      // ĐľŃĐťĐ¸ Đ´Đ˛Đ° ŃĐ°Đˇa [2,2,4,4] ŃĐľŃĐ˛ĐľŃĐ˝ŃŃ ŃĐľĐťĐľĐşŃĐ¸Ń Đ˛Đ˛ĐžĐ´Đ¸ŃŃ ŃĐž ĐąŃĐ´ĐľŃ ĐžŃĐ¸ĐąĐşĐ°
    int i = 0;
    int j = 0;
    int row = 0;
    int max = 0;
    selection (a, &i, &row, &j, &R1, &C1, &R2, &C2, &max);
    for (; i < row; i++)
    {
        if (C1 == 0)
        {
            j = 0;
            max = a->max_size;
        }
        else
        {
            j = C1-1;
            max = C1;           
        }

        if (C2 > -1)
        {
            max = C2;
        }

        if (C2 == 0)
        {
            max = a->max_size;
        }

        for(; j < max; j++)
        {
            w->words[i][j][0] = '\0';
        }
    }
}

void icol (tab *w, var *a, free_var *b, int C1, int s) 
{
    trimming_table (w, a, b, s);               
    for (int i = 0; i < a->cnt_row; i++)            
    {
        for (int k = a->max_size-2; k >= C1-1; k--)
        {
            strcpy(w->words[i][k+1], w->words[i][k]);
        }     
    }

    for (int i = 0; i < a->cnt_row; i++)
    {
            w->words[i][C1-1][0] = '\0';       
    }
}

void acol (tab *w, var *a, free_var *b, int C1, int s)
{
    trimming_table (w, a, b, s);               
    for (int i = 0; i < a->cnt_row; i++)           
    {
        for (int k = a->max_size-2; k >= C1-1; k--)
        {
            strcpy(w->words[i][k+1], w->words[i][k]);
        }
    }

    for (int i = 0; i < a->cnt_row; i++)
    {   
            w->words[i][C1][0] = '\0';        
    }
}

void set ( tab *w, var *a, free_var *b, char **my_arg, int n, int R1, int C1, int R2, int C2)
{
    int size = strlen(my_arg[n]);  
    int i = 0;
    int j = 0;
    int row = 0;
    int max = 0;

    selection (a, &i, &row, &j, &R1, &C1, &R2, &C2, &max);

    for (; i < row; i++)
    {
        if (C1 == 0)
        {
            j = 0;
            max = a->max_size;
        }
        else
        {
            j = C1-1;
            max = C1;           
        }

        if (C2 > -1)
        {
            max = C2;
        }

        if (C2 == 0)
        {
            max = a->max_size;
        }

        for(; j < max; j++)
        {
            for (int k = 0; k < size; k++)
            {
                if (w->words[i][j][k] == '\0')
                {
                    if (size > w->inf_s[i][j])
                    {
                        w->inf_s[i][j] = size;
                    }
                    realloc_words(w, b, i+1, j+1, k+2);

                }
                w->words[i][j][k] = my_arg[n][k];
            }
            w->words[i][j][size] = '\0';
        }
    }
}

// processing as selection
void selection (var *a, int *i, int *row, int *j, int *R1, int *C1, int *R2, int *C2, int *max)
{
    if (*R1 == 0)
    {
        *i = 0;
        *row = a->cnt_row;
    }
    else
    {
        *i = *R1-1;
        *row = *R1;            
    }
        
    if (*C1 == 0)
    {
        *j = 0;
        *max = a->max_size;
    }
    else
    {
        *j = *C1-1;
        *max = *C1;           
    }

    if (*R2 > -1)
    {
        *row = *R2;
    }
    
    if (*R2 == 0)
    {
        *row = a->cnt_row;
    }

    if (*C2 > -1)
    {
        *max = *C2;
    }

    if (*C2 == 0)
    {
        *max = a->max_size;
    }
}


int read_words_from_file(char *filename, tab *w, free_var *b, var *a)
{
    FILE *f = fopen(filename, "r");
    if (f == NULL)
    {
        return 0;
    }
    write_to_array (w, b, a, f);

    // fclose(f);
    return 1;
}

// processing text and writing to an array
void write_to_array (tab *w, free_var *b, var *a, FILE *f)
{
    char symbol = getc(f);
    w->words = malloc(sizeof(char***));
    w->inf_s = malloc(sizeof(char**));
    w->inf_c = malloc(sizeof(char*));
    b->free_s = malloc(sizeof(char**));
    b->free_c = malloc(sizeof(char*));
    while (symbol != EOF)
    {
        a->cnt_row++;
        a->cnt_cell = 0;
        realloc_row (w, b, a->cnt_row, 1);
        ungetc(symbol, f);

        while (1)
        {   
            if (a->cnt_cell == 0)
            {
                malloc_cell (w, b, a->cnt_row, 1);
            }

            a->cnt_cell++;
            if(a->cnt_cell > a->max_size)
            {
                a->max_size = a->cnt_cell;
            }
                
            realloc_cell (w, b, a->cnt_row, a->cnt_cell, 1);
            w->inf_c[a->cnt_row-1] = a->cnt_cell-1;

            if (!word_to_array (w, a, b, f, symbol))
            {
                break;
            }
        }
        symbol = getc(f);
    }
}

// puts letters into an array
int word_to_array (tab *w, var *a, free_var *b, FILE *f, char symbol)
{
    a->cnt_words = 1;
    int dl = 0;
    for(;1; a->cnt_words++)
    {
        symbol = getc(f);
        if (symbol == '\0') return 0;
        if (dl != 1)
            symbol = check_delim (a, symbol);
        if (a->cnt_words-1 == 0)
            malloc_words (w, b, a->cnt_row, a->cnt_cell);

        realloc_words (w, b, a->cnt_row, a->cnt_cell, a->cnt_words+1);
        
        if (symbol == EOF) break;
        if (symbol == '\n') break;
        if (symbol == '"') dl++;
        if (dl != 1)
        {
            if (symbol == a->delim[0]) break;
        }

        w->words[a->cnt_row-1][a->cnt_cell-1][a->cnt_words-1] = symbol;
    }
    w->inf_s[a->cnt_row-1][a->cnt_cell-1] = a->cnt_words-2;
    w->words[a->cnt_row-1][a->cnt_cell-1][a->cnt_words-1] = '\0';
    if (symbol == EOF) return 0;
    if (symbol == '\n') return 0;
    else return 1;
}

void print_table (tab *w, var *a, char *filename)
{
    FILE *f = fopen(filename, "w");
    for (int i = 0; i < a->cnt_row; i++)
    {
        for(int j = 0; j < a->max_size; j++)
        {
            for (int k = 0; k < (int) strlen(w->words[i][j]); k++)
            {
                    fprintf( f,"%c", w->words[i][j][k]);
            }
            if(j < (a->max_size-1))
            {
                fprintf( f,"%c", a->delim[0]);
            }
        }
        fprintf( f,"\n");
    }
    fclose(f);
}

// align the table or expand the number of cells
void trimming_table (tab *w, var *a, free_var *b, int n)  
{
    a->max_size = a->max_size+n;

    for (int i = 1; i <= a->cnt_row; i++)
    {
        realloc_cell (w, b, i, a->max_size, 0);
    }

    int j = 0;
    for (int i = 1; i <= a->cnt_row; i++)
    {
        j = w->inf_c[i-1];
        if (w->inf_c[i-1] != (a->max_size - 1))
        {
            for (;j+1 < a->max_size; j++)
            {
                malloc_words (w, b, i, j+2);
                w->words[i-1][j+1][0] = '\0';
            }
        }
    }
}

// increase in the number of rows
void row_plus (tab *w, var *a, free_var *b, int n)
{
    int q = a->cnt_row+n;

    realloc_row (w,b, q, 0);
    malloc_cell (w, b, q, 0);
    realloc_cell (w, b, q, a->max_size, 0);

    int j = 0;
    for (int i = a->cnt_row+1; i <= a->cnt_row+n; i++)
    {
        j = 0;

        for (;j+1 <= a->max_size; j++)
        {
            malloc_words (w, b, i, j+1);
            w->words[i-1][j][0] = '\0';
        }
    }
}

// expanding array lines
void realloc_row (tab *w, free_var *b, int cnt_row, int sw)
{
    w->words = realloc(w->words, sizeof(char*) * cnt_row);
    if (sw == 1)
    {
        w->inf_s = realloc(w->inf_s, sizeof(char*) * cnt_row);
        w->inf_c = realloc(w->inf_c, sizeof(char*) * cnt_row);
    }
        b->free_s = realloc(b->free_s, sizeof(char*) * cnt_row);
        b->free_c = realloc(b->free_c, sizeof(char*) * cnt_row);
        b->free_r = b->free_r + 1;
}

// expansion cell array 
void realloc_cell (tab *w, free_var *b, int cnt_row, int cnt_cell, int sw)
{
    w->words[cnt_row-1] = realloc(w->words[cnt_row-1], sizeof(char**) * cnt_cell);
    if(sw == 1)
    {
        w->inf_s[cnt_row-1] = realloc(w->inf_s[cnt_row-1], sizeof(char**) * cnt_cell);
    }
        b->free_s[cnt_row-1] = realloc(b->free_s[cnt_row-1], sizeof(char**) * cnt_cell);
        b->free_c[cnt_row-1] = b->free_c[cnt_row-1] + 1;
}

// expansion cell array 
void malloc_cell (tab *w, free_var *b, int cnt_row, int sw)
{
    w->words[cnt_row-1] = malloc(sizeof(char**));
    if (sw == 1)
    {
        w->inf_s[cnt_row-1] = malloc(sizeof(char**));
    }
        b->free_s[cnt_row-1] = malloc(sizeof(char**));
        b->free_c[cnt_row-1] = b->free_c[cnt_row-1] + 1;
}

// expanding letter array
void realloc_words (tab *w, free_var *b, int cnt_row, int cnt_cell, int cnt_words)
{
    w->words[cnt_row-1][cnt_cell-1] = realloc(w->words[cnt_row-1][cnt_cell-1], sizeof(char*) * cnt_words);
    b->free_s[cnt_row-1][cnt_cell-1] =  b->free_s[cnt_row-1][cnt_cell-1] + 1;
}

// expanding letter array
void malloc_words (tab *w, free_var *b, int cnt_row, int cnt_cell)
{
    w->words[cnt_row-1][cnt_cell-1] = malloc(sizeof(char*));
    b->free_s[cnt_row-1][cnt_cell-1] =  b->free_s[cnt_row-1][cnt_cell-1] + 1;
    
}