#include "stack.h"


int main(void)
{
    stack_t stack = {};

    STACK_CTOR(&stack, 7, "stack", __FILE__, __func__, __LINE__);

    STACK_PUSH(&stack, 76);
    STACK_PUSH(&stack, 25);
    STACK_PUSH(&stack, 96);
    STACK_PUSH(&stack, 24);
    stack_elem_t x = 0;
    STACK_POP(&stack, &x);
    STACK_POP(&stack, &x);
    STACK_PUSH(&stack, 17);
    STACK_POP(&stack, &x);
    STACK_POP(&stack, &x);
    STACK_POP(&stack, &x);
    STACK_POP(&stack, &x);
    STACK_POP(&stack, &x);
    STACK_PUSH(&stack, 222);

    STACK_DTOR(&stack, SUCCESSFUL_RETURN);

    return 0;
}