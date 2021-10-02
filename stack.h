#ifndef STACK_H
#define STACK_H

//---------------------
//!
//! Stack to store data
//!
//---------------------

struct Stack
{
    int* data;             //< pointer to the first element
    unsigned int size;     //< amount of stored elements
    unsigned int capacity; //< amount of allocated memory
};

//-----------------------------------------
//!
//! Default constructor for stack
//!
//! @param[out] <stack> pointer to stack
//! @note: constructor without params
//!
//-----------------------------------------

int stackCtor(struct Stack* stack);

//-----------------------------------------
//!
//! Destructor for stack
//!
//! @param[out] <stack> pointer to stack
//!
//-----------------------------------------

int stackDtor(struct Stack* stack);

//-----------------------------------------
//!
//! Change stack's capacity
//!
//! @param[out] <stack> pointer to stack
//! @note:    amount of realloced memory 
//!                      depends on size
//-----------------------------------------

int stackResize(struct Stack* stack);

//-----------------------------------------
//! 
//! Push element to stack
//!
//! @param[out] <stack> pointer to stack
//! @param[in]  <value> element to push
//!
//-----------------------------------------

int stackPush(struct Stack* stack, int value);

//-----------------------------------------
//!
//! Pop last-in element from stack
//! 
//! @param[out] <stack>    pointer to stack
//!
//-----------------------------------------
int stackPop(struct Stack* stack);

#endif // STACK_H
