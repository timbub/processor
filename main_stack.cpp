
#include "stack_func.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int main()
{
    Stack_t stk  = {};
    stack_ctor(&stk);
    work_with_stack(&stk);
    stack_dtor(&stk);
    return 0;
}







