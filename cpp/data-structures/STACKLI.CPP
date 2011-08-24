#include "StackLi.h"
#include "Exception.h"

template <class Etype>
const Stack<Etype> &
Stack<Etype>::operator=( const Stack<Etype> & Rhs )
{
        // Check for aliasing
    if( this == &Rhs )
        return *this;

    MakeEmpty( );
    if( Rhs.IsEmpty( ) )
        return *this;

    StackNode *Ptr = new StackNode( Rhs.TopOfStack->Element );
    StackNode *RhsPtr = Rhs.TopOfStack->Next;

    TopOfStack = Ptr;
    while( RhsPtr != NULL )
    {
        Ptr->Next = new StackNode( RhsPtr->Element );
        RhsPtr = RhsPtr->Next;
        Ptr = Ptr->Next;
    }

    return *this;
}


template <class Etype>
void
Stack<Etype>::Push( const Etype & X )
{
    TopOfStack = new StackNode( X, TopOfStack );
}

template <class Etype>
void
Stack<Etype>::Pop( )
{
    EXCEPTION( IsEmpty( ), "Can't Pop an empty stack" );

    StackNode *Old = TopOfStack;
    TopOfStack = TopOfStack->Next;
    delete Old;
}

template <class Etype>
const Etype &
Stack<Etype>::Top( ) const
{
    EXCEPTION( IsEmpty( ), "Can't Top an empty stack" );
    return TopOfStack->Element;
}

template <class Etype>
void
Stack<Etype>::MakeEmpty( )
{
    while( !IsEmpty( ) )
        Pop( );
}
