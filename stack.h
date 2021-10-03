#ifndef STACK_H
#define STACK_H

typedef int element;

//---------------------
//!
//! Stack to store data
//!
//---------------------

struct Stack
{
    element* data;         //< pointer to the first element
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
//! @param[in]  <operation> shows whether
//! program needs to make stack bigger or 
//!                               smaller
//! @return     stack capacity
//! @note:      realloced memory  amount
//!                      depends on size
//-----------------------------------------

int stackResize(struct Stack* stack, int operation);

//-----------------------------------------
//! 
//! Push element to stack
//!
//! @param[out] <stack> pointer to stack
//! @param[in]  <value> element to push
//!
//! @return pushed element
//!
//-----------------------------------------

element stackPush(struct Stack* stack, element value);

//-----------------------------------------
//!
//! Pop last-in element from stack
//! 
//! @param[out] <stack>    pointer to stack
//!
//! @return poped element
//!
//-----------------------------------------
element stackPop(struct Stack* stack);

#endif // STACK_H
