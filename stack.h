#include <stdio.h>
#include <assert.h>
#include <malloc.h>
#include <math.h>
#include "colors.h"
#include <stdbool.h>

typedef int stack_elem_t;
#define deb_spec "%d"
const stack_elem_t POISON = 666;
const stack_elem_t LEFT_CANARY = 3802;
const stack_elem_t RIGHT_CANARY = 3802;

const double EPS = 1e-5;


typedef enum error_codes
{
    SUCCESSFUL_RETURN = 0,
    MEMORY_ALLOCATION_ERROR = 10,
    NULL_STACK_MEANING = 11,
    SIZE_MORE_THAN_CAPACITY = 12,
    ZERO_CAPACITY_ERROR = 13,
    POP_FROM_EMPTY = 14,
    NULL_DATA_MEANING = 15,
    NULL_VALUE_RETURN = 16,
    POISON_ELEMENT_MENTION = 17,
    LEFT_CANARY_LOSE = 18,
    RIGHT_CANARY_LOSE = 19,
    LEFT_STACK_CANARY_LOSE = 20,
    RIGHT_STACK_CANARY_LOSE = 21
} error_codes;

#ifdef STACK_ON_DEBUG
    #define ONDBG(...) __VA_ARGS__
    #define STACK_DTOR(stack, error) {                                                                     \
                stack_dtor((stack), (error));                                                              \
                if (is_okay("STACK_DTOR", (stack), error) == ABORT_PROCESS)                                \
                {                                                                                          \
                    return error;                                                                          \
                }                                                                                          \
            }

    #define STACK_CTOR(stack, capacity, name, file, func, line) {                                          \
                error_codes error = stack_ctor((stack), (capacity), (name), __FILE__, __func__, __LINE__); \
                if (is_okay("STACK.CTOR", (stack), (error)) == ABORT_PROCESS)                              \
                {                                                                                          \
                    stack_dtor((stack), error);                                                            \
                    return error;                                                                          \
                }                                                                                          \
            }

    #define STACK_PUSH(stack, elem) {                                                                     \
                error_codes error = stack_push((stack), (elem));                                          \
                if (is_okay("STACK_PUSH", (stack), (error)) == ABORT_PROCESS)                             \
                {                                                                                         \
                    stack_dtor((stack), error);                                                           \
                    return error;                                                                         \
                }                                                                                         \
            }

    #define STACK_POP(stack, rtrn_val) {                                                                  \
                error_codes error = stack_pop((stack), (rtrn_val));                                       \
                if (is_okay("STACK_POP", (stack), error) == ABORT_PROCESS)                                \
                {                                                                                         \
                    stack_dtor((stack), error);                                                           \
                    return error;                                                                         \
                }                                                                                         \
            }

#else
    #define ONDBG(...)
    #define STACK_CTOR(stack, capacity, name, file, func, line) stack_ctor(stack, capacity)
    #define STACK_PUSH(stack, elem) stack_push(stack, elem)
    #define STACK_POP(stack, rtrn_val) stack_pop(stack, rtrn_val)
    #define STACK_DTOR(stack, error) stack_dtor(stack, error)
#endif

typedef enum conclusion
{
    ABORT_PROCESS = 0,
    CONTINUE_PROCESS = 1
} conclusion;

typedef struct stack_t
{
    const stack_elem_t lcanary  = LEFT_CANARY;
    ONDBG(const char *name; const char *file; const char *func; size_t line;)
    stack_elem_t *data;
    size_t size;
    size_t capacity;
    const stack_elem_t rcanary = RIGHT_CANARY;
} stack_t;

error_codes stack_ctor(stack_t *stack, size_t capacity ONDBG(,const char *name, const char *file, const char *func, size_t line));
error_codes stack_push(stack_t *stack, stack_elem_t elem);
error_codes stack_pop(stack_t *stack, stack_elem_t *rtrn_val);
void stack_dtor(stack_t *stack, error_codes error);
void stack_dump(stack_t *stack, const char *reason, const char *process);
error_codes check_errors(stack_t *stack);
conclusion is_okay(const char *process, stack_t *stack, error_codes error);
void poison_stack(size_t begin, size_t end, stack_t *stack);
error_codes check_allocation(stack_elem_t *data, size_t desirable_size);
