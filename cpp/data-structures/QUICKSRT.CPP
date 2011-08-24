template <class Etype>
inline void
Swap( Etype & A, Etype & B )
{
    Etype Tmp = A;
    A = B;
    B = Tmp;
}

template <class Etype>
void
InsertionSort( Etype A[ ], int N )
{
    for( int P = 1; P < N; P++ )
    {
        Etype Tmp = A[ P ];
        int j;

        for( j = P; j > 0 && Tmp < A[ j-1 ]; j-- )
            A[ j ] = A[ j-1 ];
        A[ j ] = Tmp;
    }
}

// Quicksort: sort first N items in array A
// Etype: must have copy constructor, operator=, and operator<

static const int Cutoff = 10;

template <class Etype>
void
QuickSort( Etype A[ ], int Low, int High )
{
    if( Low + Cutoff > High )
        InsertionSort( &A[ Low ], High - Low + 1 );
    else
    {
            //Sort Low, Middle, High
        int Middle = ( Low + High ) / 2;

        if( A[ Middle ] < A[ Low ] )
            Swap( A[ Low ], A[ Middle ] );
        if( A[ High ] < A[ Low ] )
            Swap( A[ Low ], A[ High ] );
        if( A[ High ] < A[ Middle ] )
            Swap( A[ Middle ], A[ High ] );

            // Place pivot at Position High-1
        Etype Pivot = A[ Middle ];
        Swap( A[ Middle ], A[ High - 1 ] );

            // Begin partitioning
        int i, j;
        for( i = Low, j = High - 1; ; )
        {
            while( A[ ++i ] < Pivot ) // Symantec may require { }
                ;                     // instead of ;
            while( Pivot < A[ --j ] ) // for both these
                ;                     // loops. It should not.
            if( i < j )
                Swap( A[ i ], A[ j ] );
            else
                break;
        }

            // Restore pivot
        Swap( A[ i ], A[ High - 1 ] );

        QuickSort( A, Low, i - 1 );   // Sort small elements
        QuickSort( A, i + 1, High );  // Sort large elements
    }
}

template <class Etype>
void
QuickSort( Etype A[ ], int N )
{
    QuickSort( A, 0, N - 1 );
}
