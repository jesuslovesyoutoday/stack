#include <stdio.h>
#include "user_func.h"
#include "stack.h"

void intstructPrint(struct Stack* stack, FILE* fin)
{
	for (int i = 0; i < stack->size; i++)
        fprintf(fin, "* [%d] = %d\n", i, *((int*)stack->data + i));
    for (int i = stack->size; i < stack->capacity; i++)
        fprintf(fin, "[%d]\n", i);
}
    
void charstructPrint(struct Stack* stack, FILE* fin)
{
	for (int i = 0; i < stack->size; i++)
    	fprintf(fin, "* [%d] = %c\n", i, *((char*)stack->data + i));
    for (int i = stack->size; i < stack->capacity; i++)
        fprintf(fin, "[%d]\n", i);
}

