#include <stdio.h>
#include "Vector.h"

// Indirect Shellsort; avoids extra array
// Same requirements for Etype as usual
// No exception handling here...

template <class Etype>
void
Shellsort( Etype A[ ], int N )
{
    Etype *Tmp, ShuffleTmp;
    Vector<Etype *> Ptr( N ); // Array of pointers to Etype
    Vector<int>     Loc(N );  // Array w/ correct final spots
    int i;

    for( i = 0; i < N; i++ )
        Ptr[ i ] = &A[ i ];

    for( int Gap = N / 2; Gap > 0;
        Gap = Gap == 2 ? 1 : ( int ) ( Gap / 2.2 ) )
        for( i = Gap; i < N; i++ )
        {
            Tmp = Ptr[ i ];
            int j = i;

            for( ; j >= Gap && *Tmp < *Ptr[ j - Gap ]; j -= Gap )
                Ptr[ j ] = Ptr[ j - Gap ];
            Ptr[ j ] = Tmp;
        }

        // Determine Correct Final Positions
    for( i = 0; i < N; i++ )
        Loc[ i ] = Ptr[ i ] - A;

        // Shuffle it Back
    for( i = 0; i < N; i++ )
        if( Loc[ i ] != i )
        {
            int j = i, NextJ;

            for( ShuffleTmp = A[ i ]; Loc[ j ] != i; j = NextJ )
            {
                A[ j ] = A[ Loc[ j ] ];
                NextJ = Loc[ j ];
                Loc[ j ] = j;
            }
            A[ j ] = ShuffleTmp;
            Loc[ j ] = j;
        }
}
