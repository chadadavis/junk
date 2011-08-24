#include "Random.cpp"
#include "Permute.cpp"
#include <iostream.h>

main( )
{
    Random R;
    const int Size = 20;
    int A[ Size ];

    for( int i = 0; i < Size; i++ )
        cout << R.RandLong( Size,25 ) << endl;
    for( i = 0; i < Size; i++ )
        A[ i ] = 0;

    for( i = 0; i < 100000; i++ )
        A[ R.Poisson( 2.0 ) ]++;

    for( i = 0; i < Size; i++ )
        cout << A[ i ] << "\n";

    Permute( A, Size );
    for( i = 0; i < Size; i++ )
        cout << A[ i ] << "\n";
    
}
