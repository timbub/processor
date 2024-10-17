#include <stdio.h>
#include <stdlib.h>
#include "stack_func.h"

struct Code
{
    int * code;
    size_t size_code;
};

void record_code(struct Code* code_info, FILE* input_file);
void run_code(struct Stack_t* stk, struct Code* code_info);

int main(int argc, char* argv[])
{
    FILE* input_file = fopen(argv[1], "r");
    Code code_info = {};

    fscanf(input_file, "%d", &code_info.size_code);
    printf("size %d\n", code_info.size_code);

    code_info.code = (int*) calloc(code_info.size_code, sizeof(int));

    record_code(&code_info, input_file);

    Stack_t stk  = {};
    stack_ctor(&stk);
    run_code(&stk, &code_info);
    stack_dtor(&stk);
}

void record_code(struct Code* code_info, FILE* input_file)
 {
    for(size_t i = 0; i < code_info->size_code; i++)
    {
        fscanf(input_file, "%d", &code_info->code[i]);
    }
}

void run_code(struct Stack_t* stk, struct Code* code_info)
{
    for (size_t programm_options = 0; programm_options < code_info->size_code; programm_options++)
    {
        stack_elem_t x1 = 0;
        stack_elem_t x2 = 0;
        switch (code_info->code[programm_options])
        {
        case 0:
            break;
        case 1:
            printf("Current stack\n");
            for (size_t i = 0; i < stk->size; i++)
            {
                printf("%d ", stk->data[i]);
            }
            printf("\n");
            break;
        case 2: 
            stack_elem_t push;
            printf("Write number to add to the stack\n");
            scanf("%d", &push);
            stack_push(stk, push); 
            break; 
        case 3:
            x1 = stack_pop(stk);
            x2 = stack_pop(stk);
            stack_push(stk, x1 + x2);
            break;
        case 4:
            x1 = stack_pop(stk);
            x2 = stack_pop(stk);
            stack_push(stk, x1 - x2);
            break;
        case 5:
            x1 = stack_pop(stk);
            x2 = stack_pop(stk);
            stack_push(stk, x1*x2);
            break;
        case 6:
            x1 = stack_pop(stk);
            x2 = stack_pop(stk);
            stack_push(stk, x1/x2);
            break;
        case 7:
            dump(stk, stdout);
            verification(stk);
            break;
        case 8:
        if (stk->size >= 1)
        {
            stack_pop(stk);
            break;
        }
        printf("Sorry, size < 1\n");
        case 9:
        printf("programm_options %d ", programm_options);
            stack_push(stk, code_info->code[programm_options + 1]);
            programm_options ++;
            break;
        default:
        printf("Wrong options %d\n", code_info->code[programm_options]);
            break;
        }
    }
    
}