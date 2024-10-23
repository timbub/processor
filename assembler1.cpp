#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum functions
{
    HLT,
    OUT,
    IN,
    ADD,
    SUB,
    MUL,
    DIV,
    INFO,
    POP, 
    PUSH,
    JMP   
};

enum registers_code
{
    AX = 0,
    BX = 1,
    CX = 2,
    DX = 3,
    EX = 4
};

struct registers
{
    enum registers_code code;
    char* name;
};

struct n_functions
{
    enum functions code;
    char* name;
};

const size_t SIZE_LABELS = 10;
const size_t NUMBER_OF_FUNC = 11;
const size_t SIZE_REGISTERS = 10;

void assembly(FILE* input_file, FILE* output_file, int* buffer, struct n_functions* command, struct Labels* labels);
size_t get_size(FILE* input_file);

struct Label 
{
    char name[256]; 
    size_t address;
};

struct Labels 
{
    struct Label* buffer_labels;
    size_t size;
    size_t first_free_place;
};

int main(int argc, char* argv[])
{
    FILE* input_file = fopen(argv[1], "r"); 
    FILE* output_file = fopen(argv[2], "w");
    if (input_file == NULL) fprintf(stderr, "file %s is not open", argv[1]); 
    if (output_file == NULL) fprintf(stderr, "file %s is not open", argv[2]); 

    size_t size_file = get_size(input_file);

    struct n_functions options[] = {{HLT, "hlt"}, {OUT, "out"}, {IN, "in"}, {ADD, "add"}, {SUB, "sub"},
              {MUL, "mul"}, {DIV, "div"}, {INFO, "info"}, {POP, "pop"}, {PUSH, "push"}, {JMP, "jmp"}};

    int* buffer = (int*) calloc(size_file, sizeof(char));

    struct Labels labels = {};
    labels.size = SIZE_LABELS;
    labels.first_free_place = 0;
    labels.buffer_labels = (struct Label*) calloc(labels.size, sizeof(struct Label));

    struct registers array_registers[] = {{AX, "ax"}, {BX, "bx"}, {CX, "cx"}, {DX, "dx"}, {EX, "ex"}};
    
    assembly(input_file, output_file, buffer, options, &labels);

    fclose(input_file);
    fclose(output_file);
    free(labels.buffer_labels);
    return 0;
}

size_t get_size(FILE* input_file)
{
    fseek(input_file, 0L, SEEK_END); 
    size_t size_file = ftell(input_file);
    fseek(input_file, 0L, SEEK_SET); 
    return size_file;
}

void assembly(FILE* input_file, FILE* output_file, int* buffer, struct n_functions* command, struct Labels* labels) 
{
    char semicolon[] = ";";
    char string[256];
    int number_input = 0;
    int size_code = 0;
    while(fscanf(input_file, "%s", string) != EOF)
    {
        for(int i = 0; i < NUMBER_OF_FUNC; i++)
        {
            
            if (strcmp(command[i].name, string) == 0)
            {
                buffer[size_code] = command[i].code;
                size_code++;
                if (command[i].code == 9)
                {
                    fscanf(input_file, "%d", &number_input);
                    buffer[size_code] = number_input;
                    size_code++;
                    
                }
                break;
            }
        }
        if (strstr(string, semicolon) != NULL)
        {
            size_t check_label_flag = 0;
            for (int i = 0; i < labels->first_free_place; i++)
            {
                if (strcmp(string,labels->buffer_labels[i].name) == 0)
                    {
                        buffer[size_code] = labels->buffer_labels[i].address;
                        size_code++; 
                        check_label_flag = 1; 
                        break;  
                    }
            }
            if (labels->first_free_place < labels->size && check_label_flag == 0)
            {
                    strcpy(labels->buffer_labels[labels->first_free_place].name, string);
                    labels->buffer_labels[labels->first_free_place].address = size_code;
                    labels->first_free_place++;
            } 
        }

    }
    fprintf(output_file, "%d ", size_code);
    for (int i = 0; i < size_code; i++)
    {
        fprintf(output_file, "%d ", buffer[i]);
    }
}


