#include <iostream.h>

// Merge routine
// Etype: requirements described in MergeSort
//
// LeftPos = start of left half.
// RightPos = start of right half.

template <class Etype>
void
Merge( Etype A[ ], Etype TmpArray[ ], int LeftPos,
                        int RightPos, int RightEnd )
{
    int LeftEnd = RightPos - 1;
    int TmpPos  = LeftPos;
    int NumElements = RightEnd - LeftPos + 1;

    // Main loop.
    while( LeftPos <= LeftEnd && RightPos <= RightEnd )
        if( A[ LeftPos ] <= A[ RightPos ] )
            TmpArray[ TmpPos++ ] = A[ LeftPos++ ];
        else
            TmpArray[ TmpPos++ ] = A[ RightPos++ ];

    while( LeftPos <= LeftEnd )   // Copy rest of first half.
        TmpArray[ TmpPos++ ] = A[ LeftPos++ ];

    while( RightPos <= RightEnd ) // Copy rest of second half.
        TmpArray[ TmpPos++ ] = A[ RightPos++ ];

    // Copy TmpArray back.
    for( int i = 0; i < NumElements; i++, RightEnd-- )
        A[ RightEnd ] = TmpArray[ RightEnd ];
}

// MregeSort: sort first N items in array A
// Etype: must have zero-parameter constructor,
//     operator=, and operator<=
//
// Recursive Mergesort and Merge are supporting routines

template <class Etype>
void
MergeSort( Etype A[ ], Etype TmpArray[ ], int Left, int Right )
{
    if( Left < Right )
    {
        int Center = ( Left + Right ) / 2;

        MergeSort( A, TmpArray, Left, Center );
        MergeSort( A, TmpArray, Center + 1, Right );
        Merge( A, TmpArray, Left, Center + 1, Right );
    }
}

template <class Etype>
void
MergeSort( Etype A[ ], int N )
{
  try
  {
    Etype *TmpArray = new Etype[ N ];
    MergeSort( A, TmpArray, 0, N - 1 );
    delete [ ] TmpArray;
  }

  catch( ... )
  {
    cerr << "Out of memory!! Sort fails." << endl;
  }
}
