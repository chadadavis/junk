#include "BitArray.h"

// Definition for the basic constructor

BitArray::BitArray( int Size )
{
    ArraySize = ( Size + IntSize - 1 ) / IntSize;
    TheArray = new int[ ArraySize ];
    N = Size;
    ClearAllBits( );
}

void
BitArray::ClearAllBits( )
{
    for( int i = 0; i < ArraySize; i++ )
        TheArray[ i ] = 0;
}

void
BitArray::SetBit( int i )
{
    if( IsInRange( i ) )
        TheArray[ i / IntSize ] |= ( 1 << ( i % IntSize ) );
}

void
BitArray::ClearBit( int i )
{
    if( IsInRange( i ) )
        TheArray[ i / IntSize ] &= ~( 1 << ( i % IntSize ) );
}

int
BitArray::GetBit( int i ) const
{
    if( !IsInRange( i ) )
        return 0;

    return ( TheArray[ i / IntSize ] & ( 1 << ( i % IntSize ) ) ) != 0;
}
