#include "stack.h"
#include <assert.h>
#include <cstddef>
#include <cstring>
#include <cstdlib>
#include <stdio.h>

#define ZERO 0xF0
#define POINTER_POISON (element*)13
#define PUSH_POP_ERROR (element)322
#define LEFT_CANARY  0xA7EDED
#define RIGHT_CANARY 0xB1000D

int stackCtor(struct Stack* stack)
{
    assert(stack);
    stack->data         = NULL;
    stack->left_canary  = NULL; //------------ уберется в релизной версии
    stack->size         = 0;
    stack->capacity     = 0;
    stack->hash         = 0;
    stack->hash         = hashFunc(stack); //- уберется в релизной версии
    
    return 0;
}

int stackDtor(struct Stack* stack)
{
    assert(stack);
    memset(stack->data, ZERO, stack->size);
    assert(stack->data != POINTER_POISON);
    free(stack->left_canary);
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
            stack->data = (element*)malloc(4*sizeof(element) + 2*sizeof(unsigned long long int)); //- уберется в релизной версии
            //stack->data = (element*)malloc(4*sizeof(element)); ------------------------------------ добавится в релизной версии
            stack->capacity += 4;
            stack->left_canary = (unsigned long long int*)(stack->data);
            (*stack->data) = LEFT_CANARY;                                           // 
            stack->data = (element*)((unsigned long long int*)(stack->data) + 1);   // -------------- уберутся в релизной версии
            *(stack->data + stack->capacity) = RIGHT_CANARY;                        //
        }
        else
        {
            stack->left_canary = (unsigned long long int*)realloc(stack->left_canary, 
                                 (stack->size * 2)*sizeof(element) + 2*sizeof(unsigned long long int)); //- уберется в релизной версии
            //stack->data = (element*)realloc(stack->data, (stack->size * 2)*sizeof(element)); //---------- добавится в релизной версии
            stack->data = (element*)(stack->left_canary + 1); //------------------------------------------- 
            *(stack->data + stack->capacity) = (element)0;    //------------------------------------------- уберутся в релизной версии
            stack->capacity *= 2;
            *(stack->data + stack->capacity) = RIGHT_CANARY;  //------------------------------------------- уберется в релизной версии
        }
    }
    else if(stack->capacity/2 > stack->size && operation < 0)
    {
        stack->left_canary = (unsigned long long int*)realloc(stack->left_canary, 
                             (stack->capacity / 2)*sizeof(element) + 2*sizeof(unsigned long long int)); //- уберется в релизной версии
        //stack->data = (element*)realloc(stack->data, (stack->capacity / 2)*sizeof(element)); //---------- добавится в релизной версии
        stack->data = (element*)(stack->left_canary + 1); //----------------------------------------------- уберется в релизной версии
        stack->capacity /= 2;
        *(stack->data + stack->capacity) = RIGHT_CANARY;  //----------------------------------------------- уберется в релизной версии
    }
    return stack->capacity;
    
}

element stackPush(struct Stack* stack, element value)
{
    if (stackIsOk(stack) == STACK_IS_OK)
    {
        if (stackResize(stack, +1) > stack->size)
	    {
            *(stack->data + stack->size) = value;
            stack->size++;
        }
        hashFunc(stack); //--------------------------- уберется в релизной версии
        if(stackIsOk(stack) == STACK_IS_OK)
            return *(stack->data + (stack->size - 1));
    }
    return PUSH_POP_ERROR;
}

element stackPop(struct Stack* stack)
{
    if (stackIsOk(stack) == STACK_IS_OK)
    {
        element tmp = *(stack->data + (stack->size - 1));
        *(stack->data + (stack->size - 1)) = 0;
        stack->size--;
        stackResize(stack, -1);
        hashFunc(stack);//---------------------------- уберется в релизной версии
        if (stackIsOk(stack) == STACK_IS_OK)
            return tmp;
    }
    return PUSH_POP_ERROR;
}

enum stackStatus stackIsOk(struct Stack* stack)
{
    if (!stack)
        return NULL_STACK_PTR;
    if (stack->size > stack->capacity)
        return SIZE_B_CAPACITY;
    if (stack->size < 0)
        return NEGATIVE_SIZE;
    if (stack->capacity < 0)
        return NEGATIVE_CAPACITY;
    if (stack->hash != hashFunc(stack))
        return HASH_MISMATCH;
    return STACK_IS_OK;
}

char hashFunc(struct Stack* stack)
{
    char hash = 0;
    for (int i = 0; i < stack->size; i++)
    {
        hash += (((char*)stack->data)[i]);
    }
    stack->hash = hash;
    return hash;
}

//TODO: error log(?), dump
