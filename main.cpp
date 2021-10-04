#include <stdio.h>
#include "stack.h"

int main()
{
    struct Stack stack;
    stackCtor(&stack, sizeof(int));
    
    int el = 0;
    stackPush(&stack, &el);
    printf("%d\n", *((int*)stackPush(&stack, &(el = 1))));
    stackPush(&stack, &(el = 1));
    stackPush(&stack, &(el = 2));
    stackPush(&stack, &(el = 2));
    printf("%d\n", *((int*)stackPush(&stack, &(el = 6))));
    stackPush(&stack, &(el = 2));

    //printf("el1 = %d el2 = %d el3 = %d el4 = %d el5 = %d\n", el1, el2, el3, el4, el5);
    
    //int poped = stackPop(&stack);
    //printf("poped element = %d\n", poped);
    
    stackPush(&stack, &(el = 2));
    stackPush(&stack, &(el = 2));
    stackPush(&stack, &(el = 2));
    stackPush(&stack, &(el = 2));
    stackPush(&stack, &(el = 2));
    
    /*for (int i = 0; i < stack.size; i++)
    {
        printf("%d\n", stack.data[i]);
    }*/
	
	stackPop(&stack);
    stackPop(&stack);
    stackPop(&stack);
    stackPop(&stack);
    stackPop(&stack);
    stackPop(&stack);
    stackPop(&stack);
    stackPop(&stack);
    stackPop(&stack);
    
    puts("dtor");
    stackDtor(&stack);
    puts("end of dtor");
    
    return 0;
} 
