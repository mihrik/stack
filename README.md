# Stack

## Description

This project is the author's implementation of a basic stack structure. It executes functions such as: STACK_CTOR,
STACK_PUSH, STACK_POP, STACK_DTOR that find their standard implementations in C++ syntax.

## Particular qualities

Program execution depends on connected compilation flag "STACK_ON_DEBUG", parts of program that rely on this flag provided
with macros "ONDBG" that includes necessary code.

Structure "stack_t" used as the core structure of the program, its inner part:
    const stack_elem_t lcanary  = LEFT_CANARY;
    ONDBG(const char *name; const char *file; const char *func; size_t line;)
    stack_elem_t *data; - directly stack
    size_t size; - current quantity of elements
    size_t capacity; - current maximum of elements in stack
    const stack_elem_t rcanary = RIGHT_CANARY;

## Data protection

- Fill data with POISON meanings if they mustn't be used
- Anytime when program allocates the memory it checks that allocation or returns in main error code 10
- Program checks if &stack != NULL while executing any function or returns in main error code 11
- Program checks if size is less than capacity while executing any function or returns in main error code 12
- Program checks if capacity != 0 while executing any function or returns in main error code 13
- Program checks if stack is not empty while executing STACK_POP or returns in main error code 14
- Program checks if &stack_data != NULL while executing any function or returns in main error code 15
- Program checks if popped value is not NULL while executing STACK_POP or returns in main error code 16
- Program checks if popped element != POISON while executing STACK_POP or returns in main error code 17
- Program checks if canary standing for the first data element == header meaning while executing any function or returns in main error code 18
- Program checks if canary standing for the last data element == header meaning while executing any function or returns in main error code 19
- Program checks if canary standing for the first stack element == header meaning while executing any function or returns in main error code 20
- Program checks if canary standing for the last stack element == header meaning while executing any function or returns in main error code 21