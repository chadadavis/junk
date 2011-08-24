#include <iostream.h>
#include <stdlib.h>
#include "Vector.h"

// These routines do not bother to maintain the location
// Of the maximum subsequence.
//
// Etype: must have constructor from int, must have
//     += and > defined, must have copy constructor
//     and operator=, and must have all properties
//     needed for Vector

// The simple O( N^3 ) algorithm

template <class Etype>
Etype
MaxSubsequenceSum1( const Vector<Etype> & A, int N )
{
    Etype MaxSum = 0;

    for( int i = 0; i < N; i++ )
        for( int j = i; j < N; j++ )
        {
            Etype ThisSum = 0;
            for( int k = i; k <= j; k++ )
                ThisSum += A[ k ];

            if( ThisSum > MaxSum )
                MaxSum = ThisSum;
        }

    return MaxSum;
}

// A better O( N^2 ) algorithm

template <class Etype>
Etype
MaxSubsequenceSum2( const Vector<Etype> & A, int N )
{
    Etype MaxSum = 0;

    for( int i = 0; i < N; i++ )
    {
        Etype ThisSum = 0;
        for( int j = i; j < N; j++ )
        {
            ThisSum += A[ j ];

            if( ThisSum > MaxSum )
                MaxSum = ThisSum;
        }
    }

    return MaxSum;
}

// The recursive algorithm
// O( N log N )

template <class Etype>
Etype
Max3( const Etype & A, const Etype & B, const Etype & C )
{
	return A > B ? 
		( A > C ? A : C  ) :
		( B > C ? B : C );
}

template <class Etype>
Etype
MaxSubSum( const Vector<Etype> & A, int Left, int Right )
{
    Etype MaxLeftBorderSum = 0, MaxRightBorderSum = 0;
    Etype LeftBorderSum = 0, RightBorderSum = 0;
    int Center = ( Left + Right ) / 2;

    if( Left == Right ) 	// Base Case.
        return A[ Left ] > 0 ? A[ Left ] : 0;

    Etype MaxLeftSum  = MaxSubSum( A, Left, Center );
    Etype MaxRightSum = MaxSubSum( A, Center + 1, Right );

    for( int i = Center; i >= Left; i-- )
    {
        LeftBorderSum += A[ i ];
        if( LeftBorderSum > MaxLeftBorderSum )
            MaxLeftBorderSum = LeftBorderSum;
    }

    for( int j = Center + 1; j <= Right; j++ )
    {
        RightBorderSum += A[ j ];
        if( RightBorderSum > MaxRightBorderSum )
            MaxRightBorderSum = RightBorderSum;
    }

    return Max3( MaxLeftSum, MaxRightSum,
                MaxLeftBorderSum + MaxRightBorderSum );
}

// Publicly used routine

template <class Etype>
Etype
MaxSubsequenceSum3( const Vector<Etype> & A, int N )
{
    return N > 0 ? MaxSubSum( A, 0, N - 1 ) : 0;
}


template <class Etype>
Etype
MaxSubsequenceSum4( const Vector<Etype> & A, int N )
{
    Etype ThisSum = 0;
    Etype MaxSum = 0;

    for( int i = 0, j = 0; j < N; j++ )
    {
        ThisSum += A[ j ];

        if( ThisSum > MaxSum )
            MaxSum = ThisSum;
        else if( ThisSum < 0 )
        {
            i = j + 1;       // i is used only if SeqStart is needed
            ThisSum = 0;
        }
    }
    return MaxSum;
}

main( void )
{
	const int ArraySize = 1000;
	Vector<int> Test( ArraySize );

	for( int i = 0; i < ArraySize; i++ )
		Test[ i ] = rand( ) % 100 - 50;

	cout << MaxSubsequenceSum4( Test, ArraySize ) << endl;
	cout << MaxSubsequenceSum3( Test, ArraySize ) << endl;
	cout << MaxSubsequenceSum2( Test, ArraySize ) << endl;
	cout << MaxSubsequenceSum1( Test, ArraySize ) << endl;

	return 0;
}
