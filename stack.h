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
    element* data;                       //< pointer to the first element
    unsigned int size;                   //< amount of stored elements
    unsigned int capacity;               //< amount of allocated memory available for elements
    unsigned long long int* left_canary; //< pointer to the allocated memory ---------------------------- уберется в релизной версии
    char hash;                           //< outcome of hash-function ----------------------------------- уберется в релизной версии
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
//! @param[out]   <stack>  pointer to stack
//!
//! @return poped element
//!
//-----------------------------------------

element stackPop(struct Stack* stack);

//-----------------------------------------
//!
//! Enum to describe stack errors
//!
//-----------------------------------------

enum stackStatus {
    STACK_IS_OK       = 0,
    NULL_STACK_PTR    = 1,
    SIZE_B_CAPACITY   = 2,
    NEGATIVE_SIZE     = 3,
    NEGATIVE_CAPACITY = 4,
    HASH_MISMATCH     = 5
};


//-----------------------------------------
//!
//! Stack invariant, checks params' errors
//!
//! @param[in] <stack> pointer to stack
//!
//! @return type of error
//!
//-----------------------------------------

enum stackStatus stackIsOk(struct Stack* stack);

//-----------------------------------------
//!
//! Counts hash to check if data is ok
//!
//! @param[in] <stack> pointer to stack
//!
//! @return outcome of function
//!
//-----------------------------------------

char hashFunc(struct Stack* stack);

#endif // STACK_H
