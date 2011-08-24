#include <iostream.h>

unsigned long
S( unsigned int N )
{
    if( N == 1 )
        return 1;
    else
        return S( N - 1 ) + N;
}

main( )
{
    for( int i = 1; i < 10; i++ )
        cout << S( i ) << "\n";

    return 0;
}
