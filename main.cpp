#include <stdio.h>
#include "stack.h"

int main()
{
    struct Stack stack;
    stackCtor(&stack);

    int el1 = stackPush(&stack, 0);
    int el2 = stackPush(&stack, 1);
    int el3 = stackPush(&stack, 2);

    printf("el1 = %d el2 = %d el3 = %d\n", el1, el2, el3);
    
    int poped = stackPop(&stack);

    printf("poped element = %d\n", poped);
    
    stackDtor(&stack);
    
    return 0;
} 
