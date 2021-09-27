#include "stack.h"
#include <assert.h>
#include <cstddef>
#include <cstring>
#include <cstdlib>
#include <stdio.h>

#define ZERO 0xF0
#define POINTER_POISON (int*)13

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

void stackResize(struct Stack* stack) //-------------ne void
{
    if(stack->size >= stack->capacity)
    {
        if(stack->size == 0)
        {
            stack->data = (int*)calloc(4, sizeof(int));
            stack->capacity += 4;
        }
        else if (stack->size / 4 == 0)
        {
            stack->data = (int*)calloc((stack->size * 2), sizeof(int));
            stack->capacity *= 2;
        }
    }
    
}

int stackPush(struct Stack* stack, int value)
{
    stackResize(stack);
	(stack->data)[stack->size] = value;
	stack->size++;
	return (stack->data)[stack->size - 1];
}

int stackPop(struct Stack* stack)
{
    int tmp = (stack->data)[stack->size - 1];
    (stack->data)[stack->size - 1] = 0;
    stack->size--;
    //stackResize();
    return tmp;
}

//TODO: resize, typedef, error log
