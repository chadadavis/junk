#ifndef __Vector
#define __Vector

// Vector class interface: support bounds-checked arrays
//
// Etype: must have zero-parameter constructor and operator=
// CONSTRUCTION: with (a) an integer size only
//
// ******************PUBLIC OPERATIONS*********************
// =                          --> Copy if sizes are identical
// [ ]                        --> Indexing with bounds check
// int Length( )              --> Return # elements in Vector
// void Resize( int NewSize ) --> Change bounds
// void Double( )             --> Double Vector capacity

#include <iostream.h>
#include <stdlib.h>

template <class Etype>
class Vector
{
  public:
    // Constructors
    Vector( int Size );

    // Destructor
    ~Vector( )       { delete [ ] Array; }

    // Index the Array
    const Etype & operator[] ( int Index ) const;
    Etype & operator[] ( int Index );

    // Copy Identically Sized Arrays
    const Vector & operator=( const Vector & Rhs );

    // Get the Length
    int Length( ) const { return ArraySize; }

    // Resize the Array
    void Resize( int NewSize );
    void Double( ) { Resize( ArraySize * 2 ); }
  private:
    Etype * Array;
    int ArraySize;

    void GetArray( );   // Call new and check for errors

    // Disable Copy constructor
    Vector( const Vector & Rhs );
};
#endif
