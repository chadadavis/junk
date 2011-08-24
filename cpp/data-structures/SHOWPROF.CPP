// Test to see if pointer hopping is worthwhile

#include <iostream.h>

// strlen implemented with usual indexing mechanism

unsigned int
Strlen1( const char Str[ ] )
{
    unsigned int i;

    for( i = 0; Str[ i ] != '\0'; i++ )
        ;

    return i;
}

// strlen implemented with pointer hopping

unsigned int
Strlen2( const char *Str )
{
    const char *Sp = Str;

    while( *Sp++ )
        ;

    return Sp - Str - 1;
}

// Quick and dirty main

main( void )
{
    char Str[ 512 ];

    while( cin >> Str )
    {
        if( Strlen1( Str ) != Strlen2( Str ) )
            cerr << "Oops!!!" << endl;
    }

    return 0;
}
