// Quickselect: find Kth smallest of first N items in array A
// recursive routine finds Kth smallest in A[Low..High]
// Etype: must have copy constructor, oeprator=, and operator<
// Nonrecursive driver is omitted.

template <class Etype>
void
QuickSelect( Etype A[ ], int Low, int High, int k )
{
    if( Low + Cutoff > High )
        InsertionSort( &A[ Low ], High - Low + 1 );
    else
    {
            // Sort Low, Middle, High
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
            while( A[ ++i ] < Pivot )
                ;
            while( Pivot < A[ --j ] )
                ;
            if( i < j )
                Swap( A[ i ], A[ j ] );
            else
                break;
        }

            // Restore pivot
        Swap( A[ i ], A[ High - 1 ] );

            // Recurse: only this part changes
        if( k < i )
            QuickSelect( A, Low, i - 1, k );
        else if( k > i )
            QuickSelect( A, i + 1, High, k );
    }
}

template <class Etype>
void
QuickSelect( Etype A[ ], int N, int k )
{
    QuickSelect( A, 0, N - 1, k );
}
