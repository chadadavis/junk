// Stack class interface: linked list implementation
//
// Etype: must have zero-parameter and copy constructor
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

#ifndef __StackLi
#define __StackLi

#include <stdlib.h>
#include "AbsStack.h"

// List-based stack
template <class Etype>
class Stack : public AbsStack<Etype>
{
  public:
    Stack( ) : TopOfStack( NULL ) { }
    ~Stack( ) { MakeEmpty( ); }

    const Stack & operator= ( const Stack & Rhs );

    void Push( const Etype & X );
    void Pop( );
    const Etype & Top( ) const;
    int IsEmpty( ) const   { return TopOfStack == NULL; }
    int IsFull( ) const    { return 0; }
    void MakeEmpty( );
  private:
    // Copy constructor remains disabled by inheritance

    struct StackNode
    {
        Etype Element;
        StackNode *Next;
    
        StackNode( ) : Next( NULL ) { }
        StackNode( const Etype & E, StackNode *N = NULL ) :
                    Element( E ), Next( N ) { }
    };

    StackNode *TopOfStack;
};
#endif
