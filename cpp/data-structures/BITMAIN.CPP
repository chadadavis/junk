#include <iostream.h>
#include "BitArray.h"

// Cheap RandInt; works for UNIX

#include <stdlib.h>
extern "C" double drand48( );
inline int
RandInt( int Low, int High )
{
    return int( drand48( ) * ( High - Low + 1 ) + Low );
}



// Simple main program to use BitArray class
// I didn't bother with the exception handler for new here.

main( void )
{
    int StillEmpty = 0;
    int N;
    int K = 1;
    int i;

    cout << "Enter N and K: ";
    cin >> N >> K;         // Error check omitted for brevity
    BitArray B ( N );      // Declare the BitArray B
    cout << "B holds " << ( N = B.NumItems( ) ) << " bits\n";

    for( i = 0; i < K * N; i++ )
            B.SetBit( RandInt( 0, N - 1 ) );

    for( i = 0; i < N; i++ )
        if( !B.GetBit( i ) )
            StillEmpty++;

    cout << "After " << K * N << " random inserts, "
         << StillEmpty << " slots are still empty\n";

    return 0;
}
