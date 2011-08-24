#include "Hash.h"
#include <iostream.h>
#include <stdlib.h>

// Find next prime > N; assume N >= 5

int
NextPrime( int N )
{
    if( N % 2 == 0 )
        N++;

    for( ; ; N += 2 )
    {
        for( int i = 3; i * i <= N; i += 2 )
            if( N % i == 0 )
                break;
        if( i * i > N )
            return N;
    }
}

// Allocate the hash table array

template <class Etype>
void
HashTable<Etype>::AllocateArray( )
{
    Array = new HashEntry[ ArraySize ];
}

// HashTable constructor

template <class Etype>
HashTable<Etype>::HashTable( ) : ArraySize ( DefaultSize )
{
    AllocateArray( );
    CurrentSize = 0;
}

// Clear the hash table

template <class Etype>
void
HashTable<Etype>::MakeEmpty( )
{
    CurrentSize = 0;
    for( int i = 0; i < ArraySize; i++ )
        Array[ i ].Info = Empty;
}

// Return item in hash table that matches X
// Success can be tested by WasFound

template <class Etype>
const Etype &
HashTable<Etype>::Find( const Etype & X )
{
    unsigned int CurrentPos = FindPos( X );
    LastFindOK = Array[ CurrentPos ].Info == Active;
    return Array[ CurrentPos ].Element;
}

// Routine to resolve collisions and locate
// cell that must contain X if X is found

template <class Etype>
unsigned int
HashTable<Etype>::FindPos( const Etype & X ) const
{
    unsigned int i = 0;   // The number of failed probes
    unsigned int CurrentPos = Hash( X, ArraySize );

    while( Array[ CurrentPos ].Info != Empty &&
           Array[ CurrentPos ].Element != X )
    {
        CurrentPos += 2 * ++i - 1;    // Compute ith probe
        if( CurrentPos >= ArraySize ) // Implement the mod
            CurrentPos -= ArraySize;
    }

    return CurrentPos;
}

// Test if X is in the hash table

template <class Etype>
int
HashTable<Etype>::IsFound( const Etype & X ) const
{
    unsigned int CurrentPos = FindPos( X );
    return Array[ CurrentPos ].Info == Active;
}


// Return true if last Find operation was successful

template <class Etype>
int
HashTable<Etype>::WasFound( ) const
{
    return LastFindOK;
}

// Remove X from hash table
// Return true if X was removed; false otherwise

template <class Etype>
int
HashTable<Etype>::Remove( const Etype & X )
{
    unsigned int CurrentPos = FindPos( X );

    if( Array[ CurrentPos ].Info != Active )
        return 0;

    Array[ CurrentPos ].Info = Deleted;
    return 1;
}

// Insert X into hash table
// Return false if Xis a duplicate; true otherwise
// Rehash automatically as needed

template <class Etype>
int
HashTable<Etype>::Insert( const Etype & X )
{
    unsigned int CurrentPos = FindPos( X );

        // Don't insert duplicates
    if( Array[ CurrentPos ].Info == Active )
        return 0;

        // Insert X as active
    Array[ CurrentPos ] = HashEntry( X, Active );
    if( ++CurrentSize  < ArraySize / 2 )
        return 1;

        // REHASHING CODE
        // Save old table
    HashEntry *OldArray = Array;
    int OldArraySize = ArraySize;

        // Create a new double-sized, empty table
    CurrentSize = 0;
    ArraySize = NextPrime( 2 * OldArraySize );
    AllocateArray( );

        // Copy table over
    for( int i = 0; i < OldArraySize; i++ )
        if( OldArray[ i ].Info == Active )
            Insert( OldArray[ i ].Element );

        // Recycle OldArray
    delete [ ] OldArray;
    return 1;
}

// Deep copy of the hash table

template <class Etype>
const HashTable<Etype> &
HashTable<Etype>::operator=( const HashTable<Etype> & Rhs )
{
    if( this != &Rhs )
    {
        delete [ ] Array;
        ArraySize = Rhs.ArraySize;
        AllocateArray( );
        for( int i = 0; i < ArraySize; i++ )
            Array[ i ] = Rhs.Array[ i ];
        CurrentSize = Rhs.CurrentSize;
    }

    return *this;
} 

// Phoney IsEmpty routine

template <class Etype>
int
HashTable<Etype>::IsEmpty( ) const
{
    return CurrentSize == 0;   // Doesn't work with deletions
}
