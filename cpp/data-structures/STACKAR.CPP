#include "StackAr.h"
#include "Exception.h"
#include <stdlib.h>

static const int InitStackSize = 3;

template <class Etype>
Stack<Etype>::Stack( ) : MaxSize( InitStackSize ), TopOfStack( -1 )
{
    Array = new Etype[ MaxSize ];
}

template <class Etype>
const Stack<Etype> &
Stack<Etype>::operator=( const Stack<Etype> & Rhs )
{
        // Check for aliasing
    if( this == &Rhs )
        return *this;

        // Get some memory
    delete [ ] Array;
    Array = new Etype[ Rhs.MaxSize ];

        // Do the copy
    MaxSize = Rhs.MaxSize;
    TopOfStack = Rhs.TopOfStack;
    for( int i = 0; i <= TopOfStack; i++ )
        Array[ i ] = Rhs.Array[ i ];

    return *this;
}

template <class Etype>
void
Stack<Etype>::Push( const Etype & X )
{
    if( TopOfStack + 1 == MaxSize )
    {
        Etype *Old = Array;
        Array = new Etype [ MaxSize *= 2 ];

        for( int i = 0; i <= TopOfStack; i++ )
            Array[ i ] = Old[ i ];
        delete [ ] Old;
    }

    Array[ ++TopOfStack ] = X;
}

template <class Etype>
void
Stack<Etype>::Pop( )
{
    EXCEPTION( IsEmpty( ), "Can't Pop an empty stack" );
    TopOfStack--;
}

template <class Etype>
const Etype &
Stack<Etype>::Top( ) const
{
    EXCEPTION( IsEmpty( ), "Can't Top an empty stack" );
    return Array[ TopOfStack ];
}
