#ifndef __BITARRAY
#define __BITARRAY

// BitArray class: support access to an array of bits
//
// CONSTRUCTION: with (a) no initializer or (b) an integer
//     that specifies the number of bits
// All copying of BitArray objects is DISALLOWED
//
// ******************PUBLIC OPERATIONS*********************
// void ClearAllBits( )   --> Set all bits to zero
// void SetBit( int i )   --> Turn bit i on
// void ClearBit( int i ) --> Turn bit i off
// int GetBit( int i )    --> Return status of bit i
// int NumItems( )        --> Return capacity of bit array

#include <iostream.h>

class BitArray
{
  public:
    // Constructor
    BitArray( int Size = 320 );     // Basic constructor

    // Destructor
    ~BitArray( ) { delete [ ] TheArray; }

    // Member Functions
    void ClearAllBits( );
    void SetBit( int i );
    void ClearBit( int i );
    int  GetBit( int i ) const;
    int  NumItems( ) const { return N; }

  private:
    int *TheArray;             // The bit array
    int N;                     // Number of bits
    int ArraySize;             // Size of the Array

    enum { IntSize = sizeof( int ) * 8 }; // Bits in an int

    int IsInRange( int i ) const; // Check range, with error msg.


    const BitArray & operator=( const BitArray & Rhs ); // Disable copying
    BitArray( const BitArray & Right );  // Disable copy constructor
};


// Inline functions must be in interface

inline int
BitArray::IsInRange( int i ) const
{
    if( i < 0 || i >= N )
    {
        cerr << "Bit " << i << " exceeds size (" << N << ")\n";
        return 0;
    }
    return 1;
}
#endif
