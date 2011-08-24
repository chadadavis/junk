#include "Random.h"

static const long A = 48271L;
static const long M = 2147483647L;
static const long Q = M / A;
static const long R = M % A;

// RandomLong returns the next random number and updates Seed
// If on UNIX, call random instead

unsigned long
Random::RandomLong( )
{
 #ifdef unix
    return random( );
 #else
    long TmpSeed = A * ( Seed % Q ) - R * ( Seed / Q );
    if( TmpSeed >= 0 )
        Seed = TmpSeed;
    else
        Seed = TmpSeed + M;

    return Seed;
 #endif
}

double
Random::RandomReal( )
{
    return RandomLong( ) / 2147483647.0;
}

// Not best algorithm for linear congruential generators
long
Random::RandLong( long Low, long High )
{
    return RandomLong( ) % ( High - Low + 1 ) + Low;
}

#include <math.h>

unsigned
Random::Poisson( double ExpectedValue )
{
    double Limit = exp( -ExpectedValue );
    double Product = RandomReal( );

    int Count;
    for( Count = 0; Product > Limit; Count++ )
        Product *= RandomReal( );

    return Count;
}

double
Random::NegExp( double ExpectedValue )
{
    return - ExpectedValue * log( RandomReal( ) );
}
