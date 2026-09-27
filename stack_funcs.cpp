#include "stack.h"

error_codes stack_ctor(stack_t *stack, size_t capacity ONDBG(,const char *name, const char *file, const char *func, size_t line))
{
    error_codes code = SUCCESSFUL_RETURN;
    if (stack == NULL)
        return NULL_STACK_MEANING;

    ONDBG
    (
        stack->name = name;
        stack->file = file;
        stack->func = func;
        stack->line = line;
    )
    stack->capacity = capacity;
    stack->size = 0;
    stack->data = (stack_elem_t *) malloc(capacity * sizeof(stack_elem_t));

    if (check_allocation(stack->data, capacity * sizeof(stack_elem_t)))
        code = MEMORY_ALLOCATION_ERROR;

    poison_stack(0, stack->capacity, stack);

    return code;
}

error_codes stack_push(stack_t *stack, stack_elem_t elem)
{
    error_codes code = SUCCESSFUL_RETURN;
    if ((code = check_errors(stack)) != SUCCESSFUL_RETURN)
        return code;

    if (stack->size == stack->capacity)
    {
        stack->data = (stack_elem_t *) realloc(stack->data, stack->capacity * 2 * sizeof(stack_elem_t));

        if (check_allocation(stack->data, stack->capacity * sizeof(stack_elem_t) * 2) != SUCCESSFUL_RETURN)
            return MEMORY_ALLOCATION_ERROR;

        stack->capacity *= 2;

        poison_stack(stack->size, stack->capacity, stack);
    }

    stack->data[stack->size] = elem;
    stack->size++;

    code = check_errors(stack);
    return code;
}

error_codes stack_pop(stack_t *stack, stack_elem_t *rtrn_val)
{
    error_codes code = SUCCESSFUL_RETURN;
    if ((code = check_errors(stack)) != SUCCESSFUL_RETURN)
        return code;

    if (stack->size == 0)
        return POP_FROM_EMPTY;

    stack->size--;
    *rtrn_val = stack->data[stack->size];
    stack->data[stack->size] = POISON;

    if (cmp_dbl((double)stack->size,(double)stack->capacity / 2) == 1 && stack->size != 0 && stack-> size != 1)
    {
        stack->data = (stack_elem_t *) realloc(stack->data, stack->capacity / 2 * sizeof(stack_elem_t));

        if (check_allocation(stack->data, stack->capacity * sizeof(stack_elem_t) / 2) != SUCCESSFUL_RETURN)
            return MEMORY_ALLOCATION_ERROR;

        stack->capacity /= 2;
    }

    code = check_errors(stack);

    return code;
}

void stack_dtor(stack_t *stack, error_codes error)
{
    if (error != NULL_STACK_MEANING)
    {
        if (error != NULL_DATA_MEANING)
            poison_stack(0, stack->capacity, stack);

        free(stack->data);

        stack->data = NULL;
        stack->capacity = 0;
        stack->size = 0;
    }
}

void stack_dump(stack_t *stack, const char *reason, const char *process)
{
    ONDBG(
        PRINT_COLOR(EXTRA_RED, "reason: %s, while doing: %s\n", reason, process);
        printf("stack_t \"%s\"[%p] created by %s at %s:%lu\n", stack->name, stack, stack->func, stack->file, stack->line);
        printf("{\n");
        PRINT_COLOR(BLUE, "    capacity = %lu;\n", stack->capacity);
        PRINT_COLOR(BLUE, "    size = %lu;\n", stack->size);
        PRINT_COLOR(BLUE, "    data[%p]\n", stack->data);
        for (size_t i = 0; i < stack->capacity; i++)
        {
            if (i < stack->size)
                {
                    PRINT_COLOR(GREEN, "        *[%lu] = %lf\n", i, stack->data[i]);
                }
            else
                PRINT_COLOR(ORANGE, "         [%lu] = %lf\n", i, stack->data[i]);
        }
        printf("}\n");
    )
}

error_codes check_errors(stack_t *stack)
{
    if (stack == NULL)
        return NULL_STACK_MEANING;

    if (stack->data == NULL)
        return NULL_DATA_MEANING;

    if (stack->size > stack->capacity)
        return SIZE_MORE_THAN_CAPACITY;

    if (stack->capacity == 0)
        return ZERO_CAPACITY_ERROR;

    return SUCCESSFUL_RETURN;
}

conclusion is_okay(const char *process, stack_t *stack, error_codes error)
{
    bool found_error = false;

    ONDBG(
        switch(error)
        {
            case NULL_STACK_MEANING     : PRINT_COLOR(EXTRA_RED, "reason: NULL_STACK_MEANING, while doing: %s\n", process);
                                          found_error = true;
                                          break;

            case SIZE_MORE_THAN_CAPACITY: stack_dump(stack, "SIZE_MORE_THAN_CAPACITY", process);
                                          found_error = true;
                                          break;

            case ZERO_CAPACITY_ERROR    : stack_dump(stack, "ZERO_CAPACITY_ERROR", process);
                                          found_error = true;
                                          break;

            case MEMORY_ALLOCATION_ERROR: PRINT_COLOR(EXTRA_RED, "reason: MEMORY_ALLOCATION_ERROR, while doing: %s\n", process);
                                          printf("stack_t \"%s\"[%p] created by %s at %s:%lu\n", stack->name, stack, stack->func, stack->file, stack->line);
                                          found_error = true;
                                          break;

            case POP_FROM_EMPTY         : stack_dump(stack, "POP_FROM_EMPTY", process);
                                          found_error = true;
                                          break;

            case NULL_DATA_MEANING      : PRINT_COLOR(EXTRA_RED, "reason: NULL_DATA_MEANING, while doing: %s\n", process);
                                          printf("stack_t \"%s\"[%p] created by %s at %s:%lu\n", stack->name, stack, stack->func, stack->file, stack->line);
                                          found_error = true;
                                          break;

            case SUCCESSFUL_RETURN      : break;

            default                     : PRINT_COLOR(EXTRA_RED, "reason: UNKNOWN_ERROR, while doing: %s\n", process);
                                          found_error = true;
        }
    )

    if (found_error)
        return ABORT_PROCESS;

    else
        return CONTINUE_PROCESS;
}

void poison_stack(size_t begin, size_t end, stack_t *stack)
{

    for (size_t i = begin; i < end; i++)
    {
        stack->data[i] = POISON;
    }
}

error_codes check_allocation(stack_elem_t *data, size_t desirable_size)
{
    size_t malloc_size = malloc_usable_size(data);

    if (malloc_size != desirable_size)
        return MEMORY_ALLOCATION_ERROR;

    return SUCCESSFUL_RETURN;
}

int cmp_dbl(double val1, double val2)
{
    return fabs(val1 - val2) < EPS;
}