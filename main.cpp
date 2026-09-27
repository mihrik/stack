#include "stack.h"


int main(void)
{
    stack_t stack = {};

    STACK_CTOR(&stack, 10, "stack", __FILE__, __func__, __LINE__);

    STACK_PUSH(&stack, 69);
    STACK_PUSH(&stack, 52);
    STACK_PUSH(&stack, 42);
    STACK_PUSH(&stack, 67);
    double x = 0;
    STACK_POP(&stack, &x);
    STACK_POP(&stack, &x);
    STACK_PUSH(&stack, 17);
    STACK_POP(&stack, &x);
    STACK_POP(&stack, &x);
    STACK_POP(&stack, &x);
    STACK_POP(&stack, &x);
    STACK_POP(&stack, &x);
    STACK_PUSH(&stack, 122);

    STACK_DTOR(&stack, SUCCESSFUL_RETURN);

    return 0;
}