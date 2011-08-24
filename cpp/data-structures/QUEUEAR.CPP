#include "QueueAr.h"
#include "Exception.h"
#include <stdlib.h>

static const int InitQueueSize = 3;

// Add one with wraparound
template <class Etype>
void
Queue<Etype>::Increment( int & X ) const
{
    if( ++X == MaxSize )
        X = 0;
}

template <class Etype>
Queue<Etype>::Queue( ) : MaxSize( InitQueueSize ),
                         CurrentSize( 0 ), Front( 0 ), Back( -1 )
{
    Array = new Etype [ MaxSize ];
}

template <class Etype>
const Queue<Etype> &
Queue<Etype>::operator=( const Queue<Etype> & Rhs )
{
    if( this == &Rhs )
        return Rhs;

    delete [ ] Array;
    Array = new Etype [ Rhs.MaxSize ];

    CurrentSize = Rhs.CurrentSize;
    MaxSize = Rhs.MaxSize;
    Front = 0;
    Back = CurrentSize - 1;

    for( int i = 0, j = Rhs.Front; i < CurrentSize; i++, Increment( j ) )
        Array[ i ] = Rhs.Array[ j ];

    return *this;
}

template <class Etype>
void
Queue<Etype>::DoubleQueue( void )
{
    int NewSize  = MaxSize * 2;
    Etype *Old = Array;

    Array = new Etype[ NewSize ];

    for( int i = 0, j = Front; i < CurrentSize; i++, Increment( j ) ) 
        Array[ i ] = Old[ j ];

    Front = 0;
    Back = CurrentSize - 1;
    MaxSize = NewSize;

    delete [ ] Old;
}
    
template <class Etype>
void
Queue<Etype>::Enqueue( const Etype & X )
{
    if( CurrentSize == MaxSize )
        DoubleQueue( );

    Increment( Back );
    Array[ Back ] = X;
    CurrentSize++;
}

template <class Etype>
void
Queue<Etype>::Dequeue( void )
{
    EXCEPTION( IsEmpty( ), "Queue is empty" );
    CurrentSize--;
    Increment( Front );
}

template <class Etype>
const Etype &
Queue<Etype>::GetFront( ) const
{
    EXCEPTION( IsEmpty( ), "Queue is empty" );
    return Array[ Front ];
}

template <class Etype>
void
Queue<Etype>::MakeEmpty( void )
{
    CurrentSize = 0;
    Front = 0;
    Back = -1;
}
