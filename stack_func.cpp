#include "stack_func.h"

void work_with_stack(struct Stack_t* stk, int* code, size_t pointer)
{
    printf("0 - finish the programm\n 1 - print stack\n 2 - in\n 3 - add the last 2 elements\n"
    "4 - sub last 2 elements\n 5 - mul last 2 elements 6 - div last 2 elements  7 - check info\n"); // 8 - pop the element\n 9 - PUSH ne sam 
    int programm_options = -1;
    scanf("%d", &programm_options);
    while (programm_options != 0)
    {
        stack_elem_t x1 = 0;
        stack_elem_t x2 = 0;
        verification(stk);
        switch (programm_options)
        {
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
        default:
        printf("Wrong options\n");
            break;
        }
        printf("Choose programm options\n");
        scanf("%d", &programm_options);
    }
}

void stack_ctor(struct Stack_t* stk)
{
    stk->size = 0;
    stk->capacity = INIT_STACK_SIZE;
    stk->l_stack_canary = L_CANARY;
    stk->r_stack_canary = R_CANARY;
    stk->data = (stack_elem_t*) calloc(stk->capacity + CANARY_MEMORY, sizeof(stack_elem_t));
    stk->data++;
    stk->data[-1] = L_CANARY;
    stk->data[stk->capacity] = R_CANARY;
    for (size_t i = 0; i < stk->capacity; i++)
    {
        stk->data[i] = TOXIC;
    }
    stk->hash = calculation_hash(stk->data, stk->size);
    verification(stk);
}
void up_cap(struct Stack_t* stk)
{
    stk->capacity *= CHANGE_CAPACITY_COEFFICIENT;
    stk->data--;
    stk->data = (stack_elem_t*) realloc(stk->data, (stk->capacity + CANARY_MEMORY)*sizeof(stack_elem_t)); 
    stk->data++;
    stk->data[stk->capacity] = R_CANARY;
    for (size_t i = stk->size; i < stk->capacity; i++) 
    {
        stk->data[i] = TOXIC;
    }
    verification(stk);

}

void stack_push(struct Stack_t* stk, stack_elem_t push)
{
    if (stk->capacity <= stk->size)
    {
        up_cap(stk);
    }
    stk->size++;
    stk->data[stk->size - 1] = push;
    stk->hash = calculation_hash(stk->data, stk->size);
    verification(stk);
}

stack_elem_t stack_pop(struct Stack_t* stk)
{
    verification(stk);
    if (stk->capacity > CHANGE_CAPACITY_COEFFICIENT * stk->size + CHANGE_CAPACITY_COEFFICIENT)
    { 
        low_cap(stk);
    }
    stack_elem_t pop = TOXIC;
    pop = stk->data[stk->size - 1]; 
    stk->data[stk->size - 1] = TOXIC; 
    stk->size--;
    stk->hash = calculation_hash(stk->data, stk->size);
    verification(stk);
    return pop;
}

void low_cap(struct Stack_t* stk)
{
    stk->capacity /= CHANGE_CAPACITY_COEFFICIENT;
    stk->data--;
    stk->data = (stack_elem_t*) realloc(stk->data, stk->capacity*sizeof(stack_elem_t));
    stk->data++;
    stk->data[stk->capacity] = R_CANARY;
    verification(stk);    
}

void dump(struct Stack_t* stk, FILE* in)
{
    fprintf(in, "pointer data = %p\n", stk->data);
    fprintf(in, "size stack  = %d\n", stk->size); 
    fprintf(in, "capacity stack  = %d\n", stk->capacity);
    fprintf(in, "hash = %lu\n",calculation_hash(stk->data, stk->size));
    for (size_t i = 0; i <= stk->capacity + 1; i++)
    {
        stk->data[i - 1] == TOXIC ? fprintf(in, "data[%d] = %d (TOXIC) || pointer data[%d] = %p\n", i - 1, stk->data[i - 1], i - 1 , &stk->data[i - 1]) : 
                                    fprintf(in, "data[%d] = %d || pointer data[%d] = %p\n", i - 1, stk->data[i - 1], i - 1, &stk->data[i - 1]);
    }
}

void stack_dtor(struct Stack_t* stk)
{
    free(stk->data);
}

void verification(struct Stack_t* stk)
{
    size_t error_number = 0;
    stk->input_log = fopen("log_stack.txt", "a");
    if (stk->l_stack_canary != L_CANARY) {
        error_number = 1;
        fprintf(stk->input_log, "Error: stk->l_stack_canary != L_CANARY\n");
    }
    if(stk->r_stack_canary != R_CANARY) {
        error_number = 1;
        fprintf(stk->input_log, "Error: stk->r_stack_canary != R_CANARY\n");
    }
    if (stk->data[-1] != L_CANARY) {
        error_number = 1;
        fprintf(stk->input_log, "Error: stk->l_data_canary != L_CANARY\n");
    }
    if(stk->data[stk->capacity] != R_CANARY) {
        error_number = 1;
        fprintf(stk->input_log, "Error: stk->r_data_canary != R_CANARY \n");
    }
    if(stk->data == NULL) {
        error_number = 1;
        fprintf(stk->input_log, "Error: stk->data == 0\n");
    }
    if(stk->size > stk->capacity) {
        error_number = 1;
        fprintf(stk->input_log, "Error: stk->size > stk->capacity\n");
    }
    if(calculation_hash(stk->data, stk->size) != stk->hash) {
        error_number = 1;
        fprintf(stk->input_log, "Error: hash changed\n");
        
    }

    if (error_number == 1)
    {
        printf("ERROR, check log_stack.txt\n");
        dump(stk, stk->input_log);
        fprintf(stk->input_log,"%s %s\n\n\n",__TIME__, __DATE__);
        fclose(stk->input_log);
        exit(0);
    }
    fclose(stk->input_log);
}

unsigned long calculation_hash(const stack_elem_t* data, size_t size)
{
    unsigned long hash = 5381;
    for (size_t i = 0; i < size ; i++)
    {
        hash = ((hash << 5) + hash) + data[i];
    }
    return hash;
}