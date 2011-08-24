#include <stdlib.h>
#include "List.h"
#include "Ordered.h"

#ifdef __GNUC__
    #include "Ordered.cpp"
    #include "Bst.cpp"
    #include "List.cpp"
#endif

// Return the winner in the Josephus problem
// Linked list implementation

int
Jos1( int People, int Passes )
{
    List<int> L;
    ListItr<int> P = L;
    int i;

        // Construct the list
    for( i = 1; i <= People; i++ )
        P.Insert( i );

        // Play the game;
        // Note: P is always one player before
    while( People-- != 1 )
    {
        for( i = 0; i < Passes; i++ )
        {
            P++;                 // Advance
            if( !+P )            // If we just went past last player
                P.First( );      // then go back to first
        }

        if( !P.RemoveNext( ) )   // Remove next player
        {
                // RemoveNext fails if P is last item,
            P.Zeroth( );         // so for last item, set P
            P.RemoveNext( );     // to 0th player to remove first player
        }
    }

    P.First( );      // Get first and only player
    return P( );     // Return player's number
}

// Recursively construct a perfectly balanced OrderedSearchTree
// by repeated insertions in O( N log N ) time

void
BuildTree( OrderedSearchTree<int> & T, int Low, int High )
{
    int Center = ( Low + High ) / 2;

    if( Low <= High )
    {
        T.Insert( Center );
        BuildTree( T, Low, Center - 1 );
        BuildTree( T, Center + 1, High );
    }
}

// Return the winner in the Josephus problem
// Search Tree implementation

int
Jos2( int People, int Passes )
{
    OrderedSearchTree<int> T;

    BuildTree( T, 1, People );

    int Rank = 1;
    while( People > 1 )
    {
        if( ( Rank = ( Rank + Passes ) % People ) == 0 )
            Rank = People;

        T.Remove( T.FindKth( Rank ) );
        People--;
    }

    return T.FindKth( 1 );
}


// Quickie main to test

main( int argc, char **argv )
{
#ifndef NoExceptions
  try
  {
#endif

    if( argc == 3 )
    {
        cout << Jos1( atoi( argv[ 1 ] ), atoi( argv[ 2 ] ) ) << endl;
        cout << Jos2( atoi( argv[ 1 ] ), atoi( argv[ 2 ] ) ) << endl;
    }
    else
        cerr << "Usage: Jos People Passes" << endl;

#ifndef NoExceptions
  }
  catch( ... )
  {
    cerr << "Out of memory!!" << endl;
    exit( 1 );
  }
#endif

    return 0;
}

