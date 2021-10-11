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
#define LEFT_CANARY  0xB100D666
#define RIGHT_CANARY 0xB100D666
#define STRUCT_CANARY 0xCA7A7EDED



static void copy(void* el1, void* el2, size_t el_size)
{
    char* ptr1 = (char*)el1;
    char* ptr2 = (char*)el2;
    int size = el_size;
    int i = 0;
    
    
    while (size > 0)
    {
    	if (size >= sizeof(size_t))
    	{
    		((size_t*)ptr2)[i] = ((size_t*)ptr1)[i];
    		i++;
    		size -= sizeof(size_t);
    	}
    	else if (size >= sizeof(int))
    	{
    		((int*)ptr2)[i] = ((int*)ptr1)[i];
    		i++;
    		size -= sizeof(int);
    	}
    	else if (size >= sizeof(short int))
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
}

int stackCtor(struct Stack* stack, size_t el_size)
{
    assert(stack);

    #ifdef DEBUG
        stack->data = NULL;
        *((size_t*)(&stack->data) - 1) = STRUCT_CANARY;
        stack->size = 0;
        stack->capacity = 0;
        stack->el_size = el_size;
        stack->left_canary = NULL;
        stack->hash = 0;
        stack->hash = hashFunc(stack);
        *(size_t*)(&stack->hash + 1) = STRUCT_CANARY;
    #else
        stack->data = NULL;
        stack->size = 0;
        stack->capacity = 0;
        stack->el_size = el_size;
    #endif

    return 0;
}

int stackDtor(struct Stack* stack)
{
    assert(stack);
    memset(stack->data, ZERO, stack->size);
    #ifdef DEBUG
    	assert(stack->data != POINTER_POISON);
    	free(stack->left_canary);
    	stack->data        = POINTER_POISON;
    	stack->left_canary = (size_t*)POINTER_POISON;
    	stack->size = -1;
    #else
    	free(stack->data);
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
            #ifdef DEBUG
                stack->left_canary = (size_t*)malloc(4 * stack->el_size + 2 * sizeof(size_t));
                (*stack->left_canary) = LEFT_CANARY;
            	stack->data = (void*)(stack->left_canary + 1);
            	*(size_t*)((char*)stack->data + stack->capacity * stack->el_size) = RIGHT_CANARY;
            #else
                stack->data = (void*)malloc(4 * stack->el_size);
            #endif
        }
        else
        {
            stack->capacity *= 2;
            #ifdef DEBUG
                stack->left_canary = (size_t*)realloc(stack->left_canary, 
                                    (stack->size * 2) * stack->el_size + 2 * sizeof(size_t));
                stack->data = (void*)(stack->left_canary + 1);
            *(size_t*)((char*)stack->data + stack->capacity * stack->el_size) = RIGHT_CANARY;
            #else
            	stack->data = (void*)realloc(stack->data, (stack->size * 2)*(stack->el_size));
            #endif
        }
    }
    else if(stack->capacity/4 > stack->size && operation < 0)
    {
    	#ifdef DEBUG
        	stack->left_canary = (size_t*)realloc(stack->left_canary, 
            	                 (stack->capacity / 2) * stack->el_size + 2*sizeof(size_t));
            stack->data = (void*)(stack->left_canary + 1); 
            stack->capacity /= 2;
        	*(size_t*)((char*)stack->data + stack->capacity * stack->el_size) = RIGHT_CANARY;
        #else
        	stack->data = (void*)realloc(stack->data, (stack->capacity / 2) * stack->el_size);
        	stack->capacity /= 2;
        #endif
    }
    return stack->capacity;
}

void* stackPush(struct Stack* stack, void* value)
{
	#ifdef DEBUG
    	if (stackIsOk(stack) == STACK_IS_OK)
    	{
        	stackResize(stack, +1);
	    	copy(value, (void*)((char*)stack->data + stack->size * stack->el_size), stack->el_size);
        	stack->size++;
            stackDump(stack, stackIsOk(stack));
        
        	hashFunc(stack);
        	if(stackIsOk(stack) == STACK_IS_OK)
        	    return (void*)((char*)stack->data + (stack->size - 1) * stack->el_size);
            else 
            {
                stackDump(stack, stackIsOk(stack));
                return PUSH_POP_ERROR;
            }
    	}
    	else
    	{
    		stackDump(stack, stackIsOk(stack));
    		return PUSH_POP_ERROR;
    	}
    #else
    	stackResize(stack, +1);
	    copy(value, (void*)((char*)stack->data + stack->size * stack->el_size), stack->el_size);
        stack->size++;
        return (void*)((char*)stack->data + (stack->size - 1) * stack->el_size);
	#endif
}

void* stackPop(struct Stack* stack)
{
    #ifdef DEBUG
        if (stackIsOk(stack) == STACK_IS_OK)
        {   
            void* tmp = (void*)((char*)stack->data + (stack->size - 1) * stack->el_size);
            stack->size--;
            stackResize(stack, -1);
        	hashFunc(stack);
        	if (stackIsOk(stack) == STACK_IS_OK)
            	return tmp;
            else
                stackDump(stack, stackIsOk(stack));
        }
    #else
    	void* tmp = (void*)((char*)stack->data + (stack->size - 1) * stack->el_size);
        stack->size--;
        stackResize(stack, -1);
        return tmp;
    #endif 
    return PUSH_POP_ERROR;
}

#ifdef DEBUG
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

    void structPrint(struct Stack* stack, FILE* fin)
    {
        for (int i = 0; i < stack->size; i++)
            fprintf(fin, "* [%d] = %d\n", i, *((int*)stack->data + i));
        for (int i = stack->size; i < stack->capacity; i++)
            fprintf(fin, "[%d]\n", i);
    }

    void stackDump(struct Stack* stack, enum stackStatus status)
    {
        FILE* fin = fopen("dump.txt", "a");

        fprintf(fin, "//---------------------------//\n");
        fprintf(fin, "      Stack structure:\n");
        fprintf(fin, "      ----------------\n");
        fprintf(fin, "CANARY      - |0x%08x|\n", *((size_t*)(&stack->data) - 1));
        fprintf(fin, "DATA        - |%p|\n", stack->data);
        fprintf(fin, "SIZE        - |%d|\n", stack->size);
        fprintf(fin, "CAPACITY    - |%d|\n", stack->capacity);
        fprintf(fin, "EL_SIZE     - |%d|\n", stack->el_size);
        fprintf(fin, "LEFT_CANARY - |%p|\n", stack->left_canary);
        fprintf(fin, "HASH        - |0x%08x|\n", stack->hash);
        fprintf(fin, "CANARY      - |0x%08x|\n\n", *(size_t*)(&stack->hash + 1));
		fprintf(fin, "//---------------------------//\n\n");
        fprintf(fin, "Stack <%s> at: [%p], hash = 0x%08x, ERROR: %d\n\n", TYPE, stack->data, stack->hash, status);
        fprintf(fin, "-> [CANARY] = 0x%08x\n", *(stack->left_canary));
    
        void* ptr = stack->data;
    
        #ifdef STRUCT_PRINT
            structPrint(stack, fin);
        #else
            for (int i = 0; i < stack->size * stack->el_size; i++)
                fprintf(fin, "* [%d] = 0x%08x\n", i, *((char*)stack->data + i));
        #endif
    
        fprintf(fin, "-> [CANARY] = 0x%08x\n", *((size_t*)((char*)stack->data + stack->capacity * stack->el_size)));
        fprintf(fin, "//-----------------------------//\n\n\n");
        fflush(fin);
        fclose(fin);
    }
#endif
