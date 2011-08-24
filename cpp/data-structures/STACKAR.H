// Stack class interface: array implementation
//
// Etype: must have zero-parameter constructor and operator=
// CONSTRUCTION: with (a) no initializer;
//     copy construction of Stack objects is DISALLOWED
// Deep copy is supported
//
// ******************PUBLIC OPERATIONS*********************
// void Push( Etype X )   --> Insert X
// void Pop( )            --> Remove most recently inserted item
// Etype Top( )           --> Return most recently inserted item
// int IsEmpty( )         --> Return 1 if empty; else return 0
// int IsFull( )          --> Return 1 if full; else return 0
// void MakeEmpty( )      --> Remove all items
// ******************ERRORS********************************
// Predefined exception is propogated if new fails
// EXCEPTION is called for Top or Pop on empty stack

#ifndef __StackAr
#define __StackAr

#include "AbsStack.h"

template <class Etype>
class Stack : public AbsStack<Etype>
{
  public:
    Stack( );
    ~Stack( ) { delete [ ] Array; }

    const Stack & operator=( const Stack & Rhs );

    void Push( const Etype & X );
    void Pop( );
    const Etype & Top( ) const;
    int IsEmpty( ) const { return TopOfStack == -1; }
    int IsFull( ) const  { return 0; }
    void MakeEmpty( )    { TopOfStack = -1; }
  private:
    // Copy constructor remains disabled by inheritance

    int MaxSize;
    int TopOfStack;
    Etype *Array;
};
#endif
