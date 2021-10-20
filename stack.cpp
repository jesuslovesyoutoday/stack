#include "stack.h"
#include "user_func.h"
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

int stackCtor_(struct Stack* stack, size_t el_size)
{
    assert(stack);

    stack->data = NULL;
    #ifdef CANARY
        *((size_t*)(&stack->data) - 1) = STRUCT_CANARY;
    #endif
    stack->size = 0;
    stack->capacity = 0;
    stack->el_size = el_size;
    #ifdef CANARY
        stack->left_canary = NULL;
    #endif
    #ifdef HASH
        stack->hash = 0;
        stack->hash = hashFunc(stack);
        *(size_t*)(&stack->hash + 1) = STRUCT_CANARY;
    #elif defined CANARY
        *(size_t*)(&stack->left_canary + 1) = STRUCT_CANARY;
    #endif

    return 0;
}

int stackDtor(struct Stack* stack)
{
    assert(stack);
    memset(stack->data, ZERO, stack->size);
    #ifdef ERROR_LOG
    	assert(stack->left_canary != POINTER_POISON);
    #endif
    #ifdef CANARY
    	free(stack->left_canary);
        stack->left_canary = (size_t*)POINTER_POISON;
    #else
        free(stack->data);
    #endif
    #ifdef STACK_STATUS 
    	stack->left_canary = POINTER_POISON;
    	stack->size = -1;
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
            #ifdef CANARY
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
            #ifdef CANARY
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
    	#ifdef CANARY
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

void stackPush(struct Stack* stack, void* value)
{
	#ifdef ERROR_LOG
    	if (stackIsOk(stack) == STACK_IS_OK)
    	{
        	stackResize(stack, +1);
	    	copy(value, (void*)((char*)stack->data + stack->size * stack->el_size), stack->el_size);
        	stack->size++;
        
        	#ifdef HASH
                hashFunc(stack);
            #endif
            #ifdef DUMP
            	stackDump(stack, stackIsOk(stack));
                if (stackIsOk(stack) != STACK_IS_OK)
                    stackDump(stack, stackIsOk(stack));
            #endif
    	}
    	else
    	{
            #ifdef DUMP
    		    stackDump(stack, stackIsOk(stack));
            #endif
    	}
    #else
    	stackResize(stack, +1);
	    copy(value, (void*)((char*)stack->data + stack->size * stack->el_size), stack->el_size);
        stack->size++;
	#endif
}

void stackPop(struct Stack* stack)
{
    #ifdef ERROR_LOG
        if (stackIsOk(stack) == STACK_IS_OK)
        {   
            void* tmp = (void*)((char*)stack->data + (stack->size - 1) * stack->el_size);
            stack->size--;
            stackResize(stack, -1);
            #ifdef HASH
        	    hashFunc(stack);
            #endif
            #ifdef DUMP
        	    if (stackIsOk(stack) != STACK_IS_OK)
            	    stackDump(stack, stackIsOk(stack));
            #endif
        }
        else
        {
            #ifdef DUMP
                stackDump(stack, stackIsOk(stack));
            #endif
        }
    #else
    	void* tmp = (void*)((char*)stack->data + (stack->size - 1) * stack->el_size);
        stack->size--;
        stackResize(stack, -1);
    #endif 
}

#ifdef ERROR_LOG
    enum stackStatus stackIsOk(struct Stack* stack)
    {
        if (!stack)
            return NULL_STACK_PTR;
        if (stack->size > stack->capacity)
            return SIZE_B_CAPACITY;
        #ifdef HASH
            if (stack->hash != hashFunc(stack))
                return HASH_MISMATCH;
        #endif
        return STACK_IS_OK;
    }
#endif
#ifdef HASH
	unsigned int MurmurHash2 (char* key, unsigned int len)
	{
	  const unsigned int m = 0x5bd1e995;
	  const unsigned int seed = 0;
	  const int r = 24;

	  unsigned int h = seed ^ len;

	  const unsigned char * data = (const unsigned char *)key;
	  unsigned int k = 0;

	  while (len >= 4)
	  {
		  k  = data[0];
		  k |= data[1] << 8;
		  k |= data[2] << 16;
		  k |= data[3] << 24;

		  k *= m;
		  k ^= k >> r;
		  k *= m;

		  h *= m;
		  h ^= k;

		  data += 4;
		  len -= 4;
	  }

	  switch (len)
	  {
		case 3:
		  h ^= data[2] << 16;
		case 2:
		  h ^= data[1] << 8;
		case 1:
		  h ^= data[0];
		  h *= m;
	  };

	  h ^= h >> 13;
	  h *= m;
	  h ^= h >> 15;

	  return h;
	}
	
    unsigned int hashFunc(struct Stack* stack)
    {
        unsigned int hash = 0;
        hash = MurmurHash2((char*)stack->data, stack->size * stack->el_size);
        stack->hash = hash;
        return hash;
    }
#endif

#ifdef DUMP
    void stackDump(struct Stack* stack, enum stackStatus status)
    {
        FILE* fin = fopen("dump.txt", "a");
        setbuf(fin, NULL);
		
		fprintf(fin, "Stack <%s> at: [%p], hash = 0x%08x, ERROR: %d\n\n", TYPE, stack->data, stack->hash, status);
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
        fprintf(fin, "-> [CANARY] = 0x%08x\n", *(stack->left_canary));
    
        void* ptr = stack->data;
    
        #ifdef STRUCT_PRINT
        	stack->printStruct(stack, fin);
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
