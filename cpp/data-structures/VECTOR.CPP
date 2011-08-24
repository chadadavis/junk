#include "Vector.h"
#include "Exception.h"

template <class Etype>
void
Vector<Etype>::GetArray( )
{
    Array = new Etype [ ArraySize ];
}

template <class Etype>
Vector<Etype>::Vector( int Size )
{
    ArraySize = Size;
    GetArray( );
}

template <class Etype>
Etype &
Vector<Etype>::operator [ ] ( int Index )
{
    EXCEPTION( Index < 0 || Index >= ArraySize,
               "Index out of range" );
    return Array[ Index ];
}

template <class Etype>
const Etype &
Vector<Etype>::operator [ ] ( int Index ) const
{
    EXCEPTION( Index < 0 || Index >= ArraySize,
               "Index out of range" );
    return Array[ Index ];
}

template <class Etype>
const Vector <Etype> &
Vector<Etype>::operator= ( const Vector<Etype> & Rhs )
{
    if( this != &Rhs )
    {
        EXCEPTION( ArraySize != Rhs.ArraySize,
                   "Incompatable array sizes." );

        for( int i = 0; i < ArraySize; i++ )
            Array[ i ] = Rhs.Array[ i ];
    }

    return *this;
}

// Min is used in Resize

template <class Etype>
Etype
Min( const Etype & Lhs, const Etype & Rhs )
{
    return Lhs < Rhs ? Lhs : Rhs;
}

template <class Etype>
void
Vector<Etype>::Resize( int NewSize )
{
    Etype *OldArray = Array;                    // Save array
    const int MinOfOldAndNew = Min( ArraySize, NewSize );

    ArraySize = NewSize;
    GetArray( );
    for( int i = 0; i < MinOfOldAndNew; i++ )
        Array[ i ] = OldArray[ i ];
    delete [ ] OldArray;
}
