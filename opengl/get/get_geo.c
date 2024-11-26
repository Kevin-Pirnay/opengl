#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define READING 0b1
#define NEGATIF 0b10
#define FLOAT   0b100


void print_array(float *array, int size)
{
    for(int i = 0; i < size; i++)
    {
        printf("%f ", array[i]);
    }

    printf("\n");
}

void fill_buffer(char c, char *const buffer, char *const ptrb)
{
    buffer[*ptrb] = c;

    (*ptrb)++;
}

void handle_reading_whith_negatif_sign(char *const flag, char *const real_nb_raw_count)
{
    *flag |= NEGATIF;

    (*real_nb_raw_count)--;//decrement the raw of the first digit ex: 1 in 100 is in the third raw, -100 )> real_nb_raw_count = 4 => decrement give 1 as 3 for the real_nb_raw_count
}

void handle_reading_with_coma_sign(char *const flag, char *const float_raw_count, char *const real_nb_raw_count, float *const result)
{
    *flag |= FLOAT;

    (*float_raw_count)++;

    *result = *result * pow(10, -(*real_nb_raw_count + 1));/* shift the result to fit the range decimal to take in account the range after the coma and so adjust the real number range before the coma. The +1 is because the decrement is at the end of the code */
}

void handle_reading_while_float_flag(char *const float_raw_count, float *const result, const char c)
{
    *result += (c - '0') * pow(10, -(*float_raw_count)); 
            
    (*float_raw_count)++; 
}

void handle_reading_real_value(char *const real_nb_raw_count, float *const result, const char c)
{
    *result += (c - '0') * pow(10, *real_nb_raw_count); 
            
    (*real_nb_raw_count)--;
}

void handle_reading_while_negatif_flag(float *const result, char *const flag)
{
    *result = - (*result);

    *flag &= ~ NEGATIF;
}

float read_buffer(const char *const buffer, char *const ptrb)
{
    float result = 0.0f;

    char ptr = *ptrb;

    char flag = 0b0;

    char real_nb_raw_count = *ptrb - 1; //since we begin with the real value raw 0 : 10^0=1

    char float_raw_count = 0;

    for(char i = 0; i < *ptrb; i++)
    {
        if ( buffer[i] == '-' ) handle_reading_whith_negatif_sign(&flag, &real_nb_raw_count);

        else if ( buffer[i] == '.' ) handle_reading_with_coma_sign(&flag, &float_raw_count, &real_nb_raw_count, &result);
        
        else if ( flag & FLOAT ) handle_reading_while_float_flag(&float_raw_count, &result, buffer[i]);
            
        else  handle_reading_real_value(&real_nb_raw_count, &result, buffer[i]); 
    }

    if ( flag & NEGATIF )  handle_reading_while_negatif_flag(&result, &flag);

    return result;
}

void read_buffer_and_extract_result(const char *const buffer, char *const ptrb, float *const memory, char *const ptrm)
{
    float result = read_buffer(buffer, ptrb);

    memory[*ptrm] = result;

    (*ptrm)++;

    *ptrb = 0; //reset effectively buffer to 0 after extracting data;
}

void read_and_fill_memory(char c, char *const flag, char *const buffer, char *const ptrb, float *const memory, char *const ptrm)
{
    if ( c == '\n' || c == ' ' || c == '\t' || c == EOF ) 
    { 
        if ( *flag & READING ) 
        {
            read_buffer_and_extract_result(buffer, ptrb, memory, ptrm);

            *flag &= ~ READING;
        }
    }

    else
    {
        *flag |=  READING;

        fill_buffer(c, buffer, ptrb);
    }
}

void extract_data(FILE *ptrf, float *const result, const char nb_data_to_read)
{
    char flag = 0b0; //flag to handle event such as ngtive sign or coma sign

    char buffer[256] = { 0 }; //memory to put digit while reading before analyse to extract the number

    char ptrb = 0; //ptr of the buffer

    char ptrr = 0; //ptr of the result

    char c = 0b0;

    while(1)
    {
        c = fgetc(ptrf);

        if ( ptrr >= nb_data_to_read ) break;

        read_and_fill_memory(c, &flag, buffer, &ptrb, result, &ptrr);

        if ( c == EOF ) break;
    }
}

void read_file(const char *const file, float *const result, const char nb_data_to_read)
{
    FILE *ptrf = fopen(file, "r");

    if(ptrf == NULL) { printf("erreur"); return; }

    extract_data(ptrf, result, nb_data_to_read);

    fclose(ptrf);
}

// int main()
// {
//    float result[12] = { 0 };

//     read_file("input.data", result, 12);

//     print_array(result, 12);
// }