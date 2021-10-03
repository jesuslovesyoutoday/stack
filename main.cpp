#include <stdio.h>
#include "stack.h"

int main()
{
    struct Stack stack;
    stackCtor(&stack);

    int el1 = stackPush(&stack, 0);
    int el2 = stackPush(&stack, 1);
    int el3 = stackPush(&stack, 2);
    int el4 = stackPush(&stack, 3);
    int el5 = stackPush(&stack, 4);

    printf("el1 = %d el2 = %d el3 = %d el4 = %d el5 = %d\n", el1, el2, el3, el4, el5);
    
    int poped = stackPop(&stack);
    printf("poped element = %d\n", poped);
    
    int el6 = stackPush(&stack, 5);
    int el7 = stackPush(&stack, 6);
    int el8 = stackPush(&stack, 7);
    int el9 = stackPush(&stack, 8);
    int el10 = stackPush(&stack, 9);
    
    /*for (int i = 0; i < stack.size; i++)
    {
        printf("%d\n", stack.data[i]);
    }
	
	int poped1 = stackPop(&stack);
    printf("poped element = %d\n", poped1);
    int poped2 = stackPop(&stack);
    printf("poped element = %d\n", poped2);
    int poped3 = stackPop(&stack);
    printf("poped element = %d\n", poped3);
    int poped4 = stackPop(&stack);
    printf("poped element = %d\n", poped4);
    int poped5 = stackPop(&stack);
    printf("poped element = %d\n", poped5);
    
    printf("capacity = %d size = %d\n", stack.capacity, stack.size);*/
    
    puts("dtor");
    stackDtor(&stack);
    puts("end of dtor");
    
    return 0;
} 
