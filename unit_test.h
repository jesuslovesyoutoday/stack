#ifndef UNIT_TEST
#define UNIT_TEST

#include "stack.h"
#include <stack>
#include <stdio.h>
#include "user_func.h"

void unitTest1()
{

//===================================================================//

    puts("------------------------INT------------------------");
    puts("Filling stack and std::stack with the same elements\n");
    
    struct Stack stack1;
    stackCtor_(stack1, int);
    std::stack<int> stack2;
    
    for (int i = 0; i < 100; i++)
    {
        int el = i;
        stackPush(&stack1, (void*)&el);
        stack2.push(i);
    }

    int s1 = 0;
    for (int i = 100; i > 0; i--)
    {
        if (*((int*)stack1.data + i - 1) != stack2.top())
        {
            printf("ERROR: %lg_th elements are different std::stack: [%lg]  struct stack: [%lg]\n\n",
            	   i, stack2.top(), *((int*)stack1.data + i - 1));
            s1 = 1;
            break;
        }
        stack2.pop();
    }
    if (s1 == 0)
        puts("TEST PASSED\n");
    stackDtor(&stack1);
}

void unitTest2()
{
//===================================================================//
 
    puts("------------------------CHAR-----------------------");
    puts("Filling stack and std::stack with the same elements\n");
    
    struct Stack stack3;
    stackCtor_(stack3, char);
    std::stack<char> stack4;
    
    const char* ebobo = "ihatemyself";
    char* eb = (char*)ebobo;
    while (*eb != '\0')
    {
        stackPush(&stack3, eb);
        stack4.push(*eb);
        ++eb;
    }

    int s2 = 0;
    for (int i = 11; i > 0; i--)
    {
        if (*((char*)stack3.data + i - 1) != stack4.top())
        {
            printf("ERROR: %lg_th elements are different std::stack: [%c]  struct stack: [%c]\n\n", 
                   i, stack4.top(), *((char*)stack3.data + i - 1));
            s2 = 1;
            break;
        }
        stack4.pop();
    }
    if (s2 == 0)
        puts("TEST PASSED\n");
    stackDtor(&stack3);
}

#endif
