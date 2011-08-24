#include <iostream.h>
#include <stdio.h>

template <class Etype>
void
DoubleArray( Etype * & Array, int & CurrentSize )
{
    Etype * OldArray = Array;
    const int MinSize  = 5;
    int NewSize  = Array ? 2 * CurrentSize : MinSize;

    Array = new Etype [ NewSize ];
    for( int i = 0; i < CurrentSize; i++ )
        Array[ i ] = OldArray[ i ];
    CurrentSize = NewSize;

    delete [ ] OldArray;     // Safe even if OldArray is NULL
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


// Read an arbitrary number of doubles, sort them, and print them.

main( void )
{
    int *Array = NULL;          // The array
    int X;                      // An item to read
    int ItemsRead = 0;          // Number of items read so far
    int MaxSize = 0;            // Number of items Array can hold

    cout << "Enter items to sort:" << endl;
    while( cin >> X )
    {
        if( ItemsRead >= MaxSize )
            DoubleArray( Array, MaxSize );  // See text warning
        Array[ ItemsRead++ ] = X;
    }

    InsertionSort( Array, ItemsRead );

    cout << "Sorted items are: " << endl;
    for( int i = 0; i < ItemsRead; i++ )
        cout << Array[ i ] << '\n';

    return 0;
}
