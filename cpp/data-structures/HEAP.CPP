#include "Heap.h"
#include <stdlib.h>
#include "Exception.h"

// Routine to allocate the heap array

template <class Etype>
void
BinaryHeap<Etype>::GetArray( int NewMaxSize )
{
    Array = new Etype [ NewMaxSize + 1 ];
}

// Constructor for BinaryHeap

template <class Etype>
BinaryHeap<Etype>::BinaryHeap( const Etype & MinVal ) :
        MaxSize( DefaultSize ), CurrentSize( 0 ), OrderOK( 1 )
{
    GetArray( MaxSize );
    Array[ 0 ] = MinVal;
}

// If heap is full, double heap array

template <class Etype>
void
BinaryHeap<Etype>::CheckSize( )
{
    if( CurrentSize == MaxSize )
    {
        Etype *Old = Array;
        GetArray( MaxSize * 2 );
        for( int i = 0; i <= MaxSize; i++ )
            Array[ i ] = Old[ i ];
        delete [ ] Old;
        MaxSize *= 2;
    }
}

// Add X into the heap without maintaining order

template <class Etype>
void
BinaryHeap<Etype>::Toss( const Etype & X )
{
    CheckSize( );
    Array[ ++CurrentSize ] = X;
    if( X < Array[ CurrentSize / 2 ] )
        OrderOK = 0;
}

// Insert X into heap and if heap order is being maintained,
// percolate X up as needed

template <class Etype>
void
BinaryHeap<Etype>::Insert( const Etype & X )
{
    if( OrderOK == 0 )
    {
        Toss( X );
        return;
    }

    CheckSize( );

        // Percolate up
    int Hole = ++CurrentSize;
    for( ; X < Array[ Hole / 2 ]; Hole /= 2 )
        Array[ Hole ] = Array[ Hole / 2 ];
    Array[ Hole ] = X;
}

// Return minimum item in the heap
// Call FixHeap first if necessary

template <class Etype>
const Etype &
BinaryHeap<Etype>::FindMin( )
{
    EXCEPTION( IsEmpty( ), "Binary heap is empty" );

    if( OrderOK == 0 )
        FixHeap( );
    return Array[ 1 ];
}

// Delete the minimum item and place it in X

template <class Etype>
void
BinaryHeap<Etype>::DeleteMin( Etype & X )
{
    X = FindMin( );
    Array[ 1 ] = Array[ CurrentSize-- ];
    PercolateDown( 1 );
}

// Delete the minimum item; throw it away
// NOTE: It would be better to write an additional
// private member to consolidate the common work between
// the two forms of DeleteMin.

template <class Etype>
void
BinaryHeap<Etype>::DeleteMin( )
{
    Etype X;
    DeleteMin( X );
}

// Private member to percolate down in the heap

template <class Etype>
void
BinaryHeap<Etype>::PercolateDown( int Hole )
{
    int Child;
    Etype Tmp = Array[ Hole ];

    for( ; Hole * 2 <= CurrentSize; Hole = Child )
    {
        Child = Hole * 2;
        if( Child != CurrentSize &&
                Array[ Child + 1 ] < Array[ Child ] )
            Child++;
        if( Array[ Child ] < Tmp )
            Array[ Hole ] = Array[ Child ];
        else
            break;
    }
    Array[ Hole ] = Tmp;
}

// Linear time FixHeap member

template <class Etype>
void
BinaryHeap<Etype>::FixHeap( )
{
    for( int i = CurrentSize / 2; i > 0; i-- )
        PercolateDown( i );
    OrderOK = 1;
}
