#include "Random.h"
#include <sys/time.h>

// Generate a randm permutation of 1..N

void
Permute( int A[ ], int N )
{
    Random R( (int) time( 0 ) ); // Random object; seed set by time

    for( int i = 0; i < N; i++ )
        A[ i ] = i + 1;

    for( int j = 1; j < N; j++ )
    {
        // Swap with random object        
        int Pos = R.RandLong( 0, j );

        int Tmp = A[ j ];
        A[ j ] = A[ Pos ];
        A[ Pos ] = Tmp;
    }
}
