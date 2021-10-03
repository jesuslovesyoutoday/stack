#include "stack.h"
#include <assert.h>
#include <cstddef>
#include <cstring>
#include <cstdlib>
#include <stdio.h>

#define ZERO 0xF0
#define POINTER_POISON (element*)13

int stackCtor(struct Stack* stack)
{
    assert(stack);
    stack->data = NULL;
    stack->size = 0;
    stack->capacity = 0;
    
    return 0;
}

int stackDtor(struct Stack* stack)
{
    assert(stack);
    memset(stack->data, ZERO, stack->size);
    assert(stack->data != POINTER_POISON);
    free(stack->data);
    stack->data = POINTER_POISON;
    stack->size = -1;
    
    return 0;
}

int stackResize(struct Stack* stack, int operation)
{
    if(stack->size == stack->capacity && operation > 0)
    {
        if(stack->size == 0)
        {
            stack->data = (element*)calloc(4, sizeof(element));
            stack->capacity += 4;
        }
        else
        {
            stack->data = (element*)realloc(stack->data, (stack->size * 2)*sizeof(element));
            stack->capacity *= 2;
        }
    }
    else if(stack->capacity/2 > stack->size && operation < 0)
    {
        stack->data = (element*)realloc(stack->data, (stack->capacity / 2)*sizeof(element));
        stack->capacity /= 2;
    }
    return stack->capacity;
    
}

element stackPush(struct Stack* stack, element value)
{
    if (stackResize(stack, +1) > stack->size)
	{
        puts("in the if");
        *(stack->data + stack->size) = value;
        puts("after pushing");
        stack->size++;
    }
    puts("before push return");
    return *(stack->data + (stack->size - 1));
}

element stackPop(struct Stack* stack)
{
    element tmp = *(stack->data + (stack->size - 1));
    *(stack->data + (stack->size - 1)) = 0;
    stack->size--;
    stackResize(stack, -1);
    return tmp;
}

//TODO: error log, stack_is_ok, dump, canary, hash
