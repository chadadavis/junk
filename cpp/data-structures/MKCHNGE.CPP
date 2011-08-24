#include <iostream.h>
#include "Vector.h"

// Dynamic programming algorithm to solve change making problem.
// As a result, the CoinsUsed array is filled with the
// minimum number of coins needed for change from 0->MaxChange
// and LastCoin contains one of the Coins needed to make the change.

void
MakeChange( const Vector<int> & Coins, int DifferentCoins,
        int MaxChange, Vector<int> & CoinsUsed,
        Vector<int> & LastCoin )
{
    CoinsUsed[ 0 ] = LastCoin[ 0 ] = 0;

    for( int Cents = 1; Cents <= MaxChange; Cents++ )
    {
        int MinCoins = Cents, NewCoin = 0;

        for( int j = 0; j < DifferentCoins; j++ )
        {
            if( Coins[ j ] > Cents )   // Can't use coin j
                continue;
            if( CoinsUsed[ Cents - Coins[ j ] ] + 1 < MinCoins )
            {
                MinCoins = CoinsUsed[ Cents - Coins[ j ] ] + 1;
                NewCoin  = Coins[ j ];
            }
        }

        CoinsUsed[ Cents ] = MinCoins;
        LastCoin[ Cents ]  = NewCoin;
    }

}

// Simple test program
main( void )
{
        // The coins and the total amount of change
    int NumCoins = 5;
    Vector<int> Coins( NumCoins );
    Coins[ 0 ] = 1; Coins[ 1 ] = 5; Coins[ 2 ] = 10;
    Coins[ 3 ] = 21; Coins[ 4 ] = 25;
    const int Change = 87;

        // Rest of main need not be altered
    Vector<int> Used( Change + 1 );
    Vector<int> Last( Change + 1 );

    MakeChange( Coins, NumCoins, Change, Used, Last );

    cout << "Best is " << Used[ Change ]  << endl;

    for( int i = Change; i > 0; )
    {
        cout << Last[ i ] << " ";
        i -= Last[ i ];
    }
    cout << "\n";

    return 0;
}
