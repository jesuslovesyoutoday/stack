#include <stdio.h>
#include "stack.h"

int main()
{
    struct Stack stack;
    stackCtor(&stack, sizeof(int));
    
    int el = 0;
    stackPush(&stack, &el);
    stackPush(&stack, &(el = 2));
    stackPush(&stack, &(el = 3));
    stackPush(&stack, &(el = 4));
    stackPush(&stack, &(el = 6));
    stackPush(&stack, &(el = 7));
    stackPush(&stack, &(el = 8));
    stackPush(&stack, &(el = 9));
    stackPush(&stack, &(el = 10));
    stackPush(&stack, &(el = 11));
    
	stackPop(&stack);
    stackPop(&stack);
    stackPop(&stack);
    stackPop(&stack);
    stackPop(&stack);
    stackPop(&stack);
    stackPop(&stack);
    stackPop(&stack);
    stackPop(&stack);
    
    stackDtor(&stack);
    
    return 0;
} 
