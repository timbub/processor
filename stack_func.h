#ifndef __FUNC__
#define __FUNC__

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef int stack_elem_t;
const size_t INIT_STACK_SIZE = 2;
const int L_CANARY = 0xDEADF00D;
const int R_CANARY = 0xBADC0FFE;
const stack_elem_t TOXIC = -11111;
const size_t CANARY_MEMORY = 2;
const size_t CHANGE_CAPACITY_COEFFICIENT = 2;

void stack_ctor(struct Stack_t* stk);
void work_with_stack(struct Stack_t* stk);
void up_cap(struct Stack_t* stk);
void stack_push(struct Stack_t* stk, stack_elem_t push); 
stack_elem_t stack_pop(struct Stack_t* stk);  
void stack_dtor(struct Stack_t* stk); 
void dump(struct Stack_t* stk, FILE* in);
void low_cap(struct Stack_t* stk);
unsigned long calculation_hash(const stack_elem_t* data, size_t size);
void verification(struct Stack_t* stk);

struct Stack_t
{
    int l_stack_canary;
    size_t size;
    size_t capacity;
    stack_elem_t* data;
    unsigned long hash;
    FILE* input_log;
    int r_stack_canary;
};
#endif
