#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "onegin_func.h"
#include <assert.h>

enum functions
{
    HLT = 0,
    OUT,
    IN,
    ADD,
    SUB,
    MUL,
    DIV,
    INFO,
    POP,
    PUSH,
    JMP,
    PUSH_R
};

enum reg
{
    AX = 0,
    BX,
    CX,
    DX,
    EX
};

struct ident
{
    int code;
    const char* name;
};

const size_t SIZE_LABELS    = 10;
const size_t NUMBER_OF_FUNC = 11;
const size_t SIZE_REGISTERS = 10;
const size_t SIZE_STRING    = 256;
const int    ERROR_CODE     = -1;

void assembly(char** indicators, size_t size_array_indicators, int* code, struct ident* options, struct Labels* labels, struct ident* registers, FILE* output_file);
size_t get_size(FILE* input_file);
int get_value(struct ident* id, char* name);
int check_labels(struct Labels* labels, char* name, int index);
void all_ctor(struct buffer_inf* buffer_info, struct Labels* labels);
void all_dtor(struct buffer_inf* buffer_info, struct Labels* labels, char** ind, int* code);

struct Label_type
{
    char name[SIZE_STRING];
    size_t address;
};

struct Labels
{
    struct Label_type* array_labels;
    size_t size;
    size_t first_free_place;
};

int main(int argc, const char* argv[])
{
    FILE* input_file  = fopen(argv[1], "rb");
    FILE* output_file = fopen(argv[2], "wb"); //TODO read about rb and r in windows

    if (input_file  == NULL)
    {
        fprintf(stderr, "file %s is not open", argv[1]);
        return ERROR_CODE;
    }

    if (input_file  == NULL)
    {
        fprintf(stderr, "file %s is not open", argv[2]);
        return ERROR_CODE;
    }

    struct ident options[]   = {{HLT, "hlt"}, {OUT, "out"}, {IN,     "in"}, {ADD, "add"}, {SUB,  "sub"},
                                {MUL, "mul"}, {DIV, "div"}, {INFO, "info"}, {POP, "pop"}, {PUSH, "push"},
                                {JMP, "jmp"}, {PUSH_R, "push_r"}, {EOF}};

    struct ident registers[] = {{AX, "ax"},   {BX, "bx"},   {CX, "cx"},     {DX, "dx"},   {EX, "ex"}, {EOF}};

    struct buffer_inf buffer_info = {};
    buffer_info.size_buffer = get_size(input_file);
    buffer_info.buffer = NULL;

    struct Labels labels = {};
    labels.size = SIZE_LABELS;
    labels.first_free_place = 0;
    labels.array_labels = NULL;

    all_ctor(&buffer_info, &labels);

    input_buf(&buffer_info, input_file);
    buffer_info.num_string = proccess_buffer(&buffer_info);

    char** indicators = (char**) calloc(buffer_info.num_string, sizeof(char*));

    input_indicator(&buffer_info, indicators);

    int* array_code = (int*) calloc(buffer_info.num_string, sizeof(int));

    assembly(indicators, buffer_info.num_string, array_code, options, &labels, registers, output_file);

    //for(int i = 0; i < buffer_info.num_string; i++)
    //{
    //    printf("array_code[%d] %d\n ", i, array_code[i]);
    //}

    all_dtor(&buffer_info, &labels, indicators, array_code);
    fclose(input_file);
    fclose(output_file);
    return 0;
}

int get_value(struct ident* id, char* name)
{
    for(size_t j = 0; id[j].code != EOF ; j++)
    {
        if (strcmp(name, id[j].name) == 0)
        {
            return id[j].code;
        }
    }
    return -1;
}

void all_ctor(struct buffer_inf* buffer_info, struct Labels* labels)
{
    buffer_info->buffer = (char*) calloc(buffer_info->size_buffer, sizeof(char));
    labels->array_labels = (struct Label_type*) calloc(labels->size, sizeof(struct Label_type));
}

void all_dtor(struct buffer_inf* buffer_info, struct Labels* labels, char** ind, int* code)
{
    free(buffer_info->buffer);
    free(labels->array_labels);
    free(ind);
    free(code);
}

//TODO asm_struct
void assembly(char** indicators, size_t size_array_indicators, int* code, struct ident* options, struct Labels* labels, struct ident* registers, FILE* output_file) //TODO assert
{
    int index = 0;

    for(size_t i = 0; i < size_array_indicators; i++)
    {
        int up_code = get_value(options, indicators[i]);

        if (up_code != -1)
        {
            code[index++] = up_code;
        }

        if (up_code == PUSH)
        {
            code[index++] = atoi(indicators[++i]);
        } else if (up_code == PUSH_R)
        {
            int up_code_r = get_value(registers, indicators[++i]);

            if (up_code != -1)
            {
                code[index++] = up_code_r;
            }
        }

        up_code = check_labels(labels, indicators[i], index);

        if (up_code != -1)
        {
            code[index++] = up_code;
        }
    }

    fprintf(output_file, "%d\n", index);
    for(size_t i = 0; i < index; i++)
    {
        fprintf(output_file, "%d ", code[i]);
    }
   // fwrite(code, sizeof(int), index, output_file);
}

int check_labels(struct Labels* labels, char* name, int index)
{
    int colon = ':';
    if (strchr(name, colon) != NULL)
    {
        for (int j = 0; j < labels->first_free_place; j++)
        {
            if (strcmp(name, labels->array_labels[j].name) == 0)
                {
                    printf("old label - %s\n", name);
                    return labels->array_labels[j].address;
                }
        }
        if (labels->first_free_place <= labels->size)
        {
                printf("new label - %s\n", name);
                strcpy(labels->array_labels[labels->first_free_place].name, name);
                labels->array_labels[labels->first_free_place++].address = index;
                return -1;
        }
    }
    return -1;
}


