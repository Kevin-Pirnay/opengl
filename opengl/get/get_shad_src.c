#include <stdio.h>
#include <stdlib.h>

#define READING 0b1
#define F_END 0b1111 //rise when reach end of src with the symbol \*


void print_array(const char *const array, int size)
{
    for(int i = 0; i < size; i++)
    {
        printf("%c", array[i]);
    }

    printf("\n");
}

void cpy_in_memory(void *const src, void *const dest, int size)
{
    for(int i = 0; i < size; i++)
    {
        *((char* )dest + i) = *((char *)src + i);
    }
}

void print_byte_by_bit(void* obj, size_t nb_bytes)
{
    for(size_t i = 0; i < nb_bytes; i++)
    {
        char byte = *((char* )obj + i);

        for(char j = 7; j >= 0; j--)
        {
            char bit = (byte >> j) & 1;

            printf("%d", bit);
        }
        printf(" ");

        printf("%d ", byte);
    }
    printf("\n");
}

int count_white_space(const char *const line, const int size_line)
{
    int count = 0b0;

    for(int i = 0; i < size_line; i++)
    {
        if ( line[i] == ' ' ) count++;
    }

    return count;
}

int count_nb_caractere_when_trimed(const char *const line, const int size_line)
{
    int nb_white_space = count_white_space(line, size_line);

    return size_line - nb_white_space;
}

char *trim_(const char *const line, const int size_line, int *const size_result)
{
    *size_result = count_nb_caractere_when_trimed(line, size_line);

    char *result = malloc(sizeof(char) * *size_result);

    int j = 0b0;

    for(int i = 0; i < size_line; i++)
    {
        if ( line[i] == ' ' ) continue;

       result[j] = line[i];

       j++;
    }

    return result;
}

typedef struct
{
    char flag;

    int index;
}
 Match;

Match recognise_pattern(const char *const pattern, const int nb_pattern, const char *const line, const int nb_line)
{
    char flag = 0b0;

    int index = 0;

    for(int i = 0; i < nb_line; i++)
    {
        if ( pattern[0] == line[i] ) flag = 0b1;

        if ( flag )
        {            
            for(int j = 0; j < nb_pattern; j++)
            {
                if ( pattern[j] != line[i + j] ) 
                { 
                    flag = 0b0; 
                    
                    break; 
                }
            }

            if ( flag ) { index = i; break; }
        }
    }

    Match result = { flag, index };

    return result;
}

void print_str(const char *const str, const int size)
{
    for(int i = 0; i < size; i++)
    {
        printf("%c", str[i]);
    }

    printf("\n");
}

Match find_match(const char *const pattern, const int size_patt, const char *const line, const int size_line)
{
    int nb_line = 0b0;

    int nb_pattern = 0b0;

    Match result = recognise_pattern(pattern, size_patt, line, size_line);

    return result;
}

void read_and_put_the_current_line_in_memory(char *const buffer, int *const ptrb, char *const flag, const char c, const char remove_space, const char go_to_the_end)
{
    if ( ((c == ' ' || c == '\n' || c == '\t') && !(*flag & READING)) && remove_space) return;

    else if ( c == '\\' ) { *flag |= 0b10; }

    else if ( c == '*' && *flag & 0b10 && !go_to_the_end ) { *flag |= F_END; buffer[*ptrb-1] = 0; (*ptrb)--; return; }

    else if ( c != '*' && *flag & 0b10 ) { *flag &= ~0b10; }

    *flag |= READING;

    buffer[*ptrb] = c;

    (*ptrb)++;
}

typedef struct
{
    int size;

    const char *const src;
}
 File;

File save_file_in_the_heap_(char *const buffer, const int size)
{
    char *mem = malloc(sizeof(char) * size);

    cpy_in_memory(buffer, mem, size);

    File result = { size, mem };

    return result;
}

char *const allocate_buffer_to_read_file_(FILE *ptrf, const int index) 
{
    fseek(ptrf, 0, SEEK_END);

    int size = ftell(ptrf);

    fseek(ptrf, index, SEEK_SET);

    return malloc(size);
}

File read_file_from(FILE *ptrf, const int index, const char rm_wthsp, const char go_to_the_end)
{
    char *buffer = allocate_buffer_to_read_file_(ptrf, index);

    int ptrb = 0; //ptr of the buffer, memory size allocated to read the file

    char flag = 0b0;

    char c = 0b0;

    while(1)
    {
        c = fgetc(ptrf);

        read_and_put_the_current_line_in_memory(buffer, &ptrb, &flag, c, rm_wthsp, go_to_the_end);

        if ( flag == F_END ) break;

        if ( c == EOF ) break;
    } 

   File result = save_file_in_the_heap_(buffer, ptrb);

   free(buffer);

   return result;
}

int go_at_the_end_of_the_line_from(FILE *ptrf, const int index)
{
    fseek(ptrf, index, SEEK_SET);

    char c = 0b0;

    while(1)
    {
        c = fgetc(ptrf);

        if ( c == '\n' ) break;

        if ( c == EOF ) break;
    } 

    return ftell(ptrf);
}

const File extract_shader_from_file_(const char *const file_path, const char *const match)
{
    FILE *ptrf = fopen(file_path, "r");

    if(ptrf == NULL) { printf("erreur opening file"); exit(1); }

    File file = read_file_from(ptrf, 0,0b0, 0b1);

    Match result = find_match(match, sizeof(match) - 1, file.src, file.size);

    if ( !result.flag ) exit(1);

    int index = go_at_the_end_of_the_line_from(ptrf, result.index);

    File shader = read_file_from(ptrf, index, 0b0, 0b0);

    free((void *)file.src);

    fclose(ptrf);

    return shader;
}

// int main()
// {
//     File shader = extract_shader_from_file_("input.string.data", "#Vertex Shader source code");

//     print_array(shader.src, shader.size);

//     free((void *)shader.src);

//     return 0;
// }