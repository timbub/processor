#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void change_to_assembler_code(FILE* input_file, FILE* output_file, int* buffer);
size_t get_size(FILE* input_file);

const size_t NUMBER_OF_FUNC = 9;

int main(int argc, char* argv[])
{
    FILE* input_file = fopen(argv[1], "r");
    FILE* output_file = fopen(argv[2], "w");
    size_t size_file = get_size(input_file); 

    int* buffer = (int*) calloc(size_file, sizeof(int));

    change_to_assembler_code(input_file, output_file, buffer);

    fclose(input_file);
    fclose(output_file);
    return 0;
}

size_t get_size(FILE* input_file)
{
    fseek(input_file, 0L, SEEK_END);
    size_t size_file = ftell(input_file);
    fseek(input_file, 0L, SEEK_SET);
    return size_file;
}

void change_to_assembler_code(FILE* input_file, FILE* output_file, int* buffer)
{
    char* command[] = {"hlt", "out", "in", "add", "sub", "mul", "div", "info", "pop", "push"};
    char string[] = "           ";
    int number_input = 0;
    int size_code = 0;
    while(fscanf(input_file, "%s", &string) != EOF)
    {
        size_t check_command = 0;
        for(int i = 0; i <= NUMBER_OF_FUNC; i++)
        {
            
            if (strcmp(command[i], string) == 0)
            {
                buffer[size_code] = i;
                size_code++;
                if (i == 9)
                {
                    fscanf(input_file, "%d", &number_input);
                    buffer[size_code] = number_input;
                    size_code++;
                }
                break;
            }
        }    
    }
    fprintf(output_file, "%d ", size_code);
    for (int i = 0; i < size_code; i++)
    {
        fprintf(output_file, "%d ", buffer[i]);
    }
}
