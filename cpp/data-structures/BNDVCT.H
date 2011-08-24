#ifndef __BND_VECTOR
#define __BND_VECTOR

// BoundedVector class interface: Vectors with arbitrary range
//
// Etype: same requirements as Vector class Etype
// CONSTRUCTION: with (a) two integers that specify the
//     lower and upper range
//
// ******************PUBLIC OPERATIONS*********************
// All Vector operations are supported

#include "Vector.h"

template <class Etype>
class BoundedVector : public Vector<Etype>
{
  public:
    // Constructor
    BoundedVector( int L, int H ) :
        Vector<Etype>( H - L + 1 ), Low ( L ) { }

    // Index the Array
    const Etype & operator[] ( int Index ) const;
    Etype & operator[] ( int Index );
  private:
    int Low;
};
#endif
