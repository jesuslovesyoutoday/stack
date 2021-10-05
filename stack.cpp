#include "stack.h"
#include <assert.h>
#include <cstddef>
#include <cstring>
#include <cstdlib>
#include <stdio.h>
#include <stddef.h>

#define ZERO 0xF0
#define POINTER_POISON (void*)13
#define PUSH_POP_ERROR (void*)322
#define LEFT_CANARY  0xA7EDED
#define RIGHT_CANARY 0xB1000D

static void copy(void* el1, void* el2, size_t el_size)
{
    char* ptr1 = (char*)el1;
    char* ptr2 = (char*)el2;
    int size = el_size;
    int i = 0;
    
    
    while (size > 0)
    {
    	if (size >= sizeof(unsigned long long int))
    	{
    		((unsigned long long int*)ptr2)[i] = ((unsigned long long int*)ptr1)[i];
    		i++;
    		size -= sizeof(unsigned long long int);
    	}
    	else if (size >= sizeof(int))
    	{
    		((int*)ptr2)[i] = ((int*)ptr1)[i];
    		i++;
    		size -= sizeof(int);
    	}
    	else if (size >= sizeof(short int*))
    	{
    		((short int*)ptr2)[i] = ((short int*)ptr1)[i];
			i++;
			size -= sizeof(short int);
		}
		else if (size >= sizeof(char))
		{
			((char*)ptr2)[i] = ((char*)ptr1)[i];
			i++;
			size -= sizeof(char);
		}
    }
    
    /*for (int k = 0; k < el_size; k++)
    {
		ptr2[k] = ptr1[k];
	}*/
}

int stackCtor(struct Stack* stack, size_t el_size)
{
    assert(stack);
    stack->data         = NULL;
    stack->size         = 0;
    stack->capacity     = 0;
    stack->el_size = el_size;
    #if MODE == DEBUG
    {
        stack->left_canary  = NULL;
        stack->hash = 0;
        stack->hash = hashFunc(stack);
    }
    #endif
    
    return 0;
}

int stackDtor(struct Stack* stack)
{
    assert(stack);
    memset(stack->data, ZERO, stack->size);
    #if MODE == DEBUG
    {
    	assert(stack->data != POINTER_POISON);
    	free(stack->left_canary);
    	stack->data        = POINTER_POISON;
    	stack->left_canary = (unsigned long long int*)POINTER_POISON;
    	stack->size = -1;
    }
    #else
    {
    	free(stack->data);
    }
    #endif
    
    return 0;
}

int stackResize(struct Stack* stack, int operation)
{
    if(stack->size == stack->capacity && operation > 0)
    {
        if(stack->size == 0)
        {
            stack->capacity += 4;
            #if MODE == DEBUG
            {
                stack->left_canary = (unsigned long long int*)malloc(4 * stack->el_size + 2 * sizeof(unsigned long long int));
                (*stack->left_canary) = LEFT_CANARY;
            	stack->data = (void*)(stack->left_canary + 1);
            	*(unsigned long long int*)((char*)stack->data + stack->capacity * stack->el_size) = RIGHT_CANARY;
            }
            #else
                stack->data = (void*)malloc(4 * stack->el_size);
            #endif
        }
        else
        {
            stack->capacity *= 2;
            #if MODE == DEBUG
            {
                stack->left_canary = (unsigned long long int*)realloc(stack->left_canary, 
                                    (stack->size * 2) * stack->el_size + 2 * sizeof(unsigned long long int));
                stack->data = (void*)(stack->left_canary + 1);
            *(unsigned long long int*)((char*)stack->data + stack->capacity * stack->el_size) = RIGHT_CANARY;
            }
            #else
            	stack->data = (void*)realloc(stack->data, (stack->size * 2)*(stack->el_size));
            #endif
        }
    }
    else if(stack->capacity/2 > stack->size && operation < 0)
    {
    	#if MODE == DEBUG
    	{
        	stack->left_canary = (unsigned long long int*)realloc(stack->left_canary, 
            	                 (stack->capacity / 2) * stack->el_size + 2*sizeof(unsigned long long int));
            stack->data = (void*)(stack->left_canary + 1); 
            stack->capacity /= 2;
        	*(unsigned long long int*)((char*)stack->data + stack->capacity * stack->el_size) = RIGHT_CANARY;
        }
        #else
        {
        	stack->data = (void*)realloc(stack->data, (stack->capacity / 2) * stack->el_size);
        	stack->capacity /= 2;
        }
        #endif
    }
    return stack->capacity;
}

void* stackPush(struct Stack* stack, void* value)
{
	#if MODE == DEBUG
    {
    	if (stackIsOk(stack) == STACK_IS_OK)
    	{
        	stackResize(stack, +1);
	    	copy(value, (void*)((char*)stack->data + stack->size * stack->el_size), stack->el_size);
        	stack->size++;
        
        	hashFunc(stack);
        	if(stackIsOk(stack) == STACK_IS_OK)
        	    return (void*)((char*)stack->data + (stack->size - 1) * stack->el_size);
    	}
    }
    #else
    {
    	stackResize(stack, +1);
	    copy(value, (void*)((char*)stack->data + stack->size * stack->el_size), stack->el_size);
        stack->size++;
        return (void*)((char*)stack->data + (stack->size - 1) * stack->el_size);
	}
	#endif
    return PUSH_POP_ERROR;
}

void* stackPop(struct Stack* stack)
{
    if (stackIsOk(stack) == STACK_IS_OK)
    {
        void* tmp = (void*)((char*)stack->data + (stack->size - 1) * stack->el_size);
        stack->size--;
        stackResize(stack, -1);
        #if MODE == DEBUG
        {
        	hashFunc(stack);
        	if (stackIsOk(stack) == STACK_IS_OK)
            	return tmp;
        }
        #else
        	return tmp;
        #endif
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

//TODO: dump
