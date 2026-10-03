#include "util.h"
#include "math.h"
int* read_next_line(FILE* fin)
{
    char buffer_c[1024];
    int buffer_i[64];
    int i = 0;
    int cursor = 0;
    int file_char;
    while ((file_char = fgetc(fin)) != EOF) {
        if (file_char >= '0' && file_char <= '9') {
            buffer_c[cursor] = file_char;
            cursor++;
        }
        else if (file_char == ',') {
            buffer_c[cursor] = '\0';
            buffer_i[i + 1] = atoi(buffer_c);
            i++;
            cursor = 0;
        }
        else if (file_char == '\n') {
            buffer_c[cursor] = '\0';
            buffer_i[i + 1] = atoi(buffer_c);
            i++;
            int* temp_buffer = malloc(sizeof(int) * (i + 1));
            temp_buffer[0] = i;
            for (int k = 1; k <= i; k++) {
                temp_buffer[k] = buffer_i[k];
            }
            return temp_buffer;
        }
    }
    if (cursor > 0) {
        buffer_c[cursor] = '\0';
        buffer_i[i + 1] = atoi(buffer_c);
        i++;
        int* temp_buffer = malloc(sizeof(int) * (i + 1));
        temp_buffer[0] = i;
        for (int k = 1; k <= i; k++) {
            temp_buffer[k] = buffer_i[k];
        }

        return temp_buffer;
    }
    return NULL;
}


float compute_average(int* line)
{
    int accum = 0;
    for (int i = 1; i <= line[0]; i++) {
        accum += line[i];
    }
    return (float)accum / line[0];
}


float compute_stdev(int* line)
{
    float avg = compute_average(line);
    float accum = 0;
    for (int i = 1; i <= line[0]; i++) {
        accum += pow(line[i] - avg, 2);
    }
    return sqrt(accum / (line[0]));
}