#include <iostream.h>

// Print N in any Base

static char DigitTable[ ] = "0123456789abcdef";
static const int MaxBase = sizeof( DigitTable ) - 1;

static void
PrintIntRec( unsigned long N, unsigned int Base )
{
    if( N >= Base )
        PrintIntRec( N / Base, Base );
    cout << DigitTable[ N % Base ];
}

void
PrintInt( unsigned long N, unsigned int Base )
{
    if( Base <= 1 || Base > MaxBase )
        cerr << "Cannot print in base " << Base << endl;
    else
        PrintIntRec( N, Base );
}

main( )
{
    int i;

    for( i=0; i<=17; i++ )
    {
        PrintInt( 1000, i );
        cout << endl;
    }
}
