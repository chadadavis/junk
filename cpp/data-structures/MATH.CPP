#include <iostream.h>

// Compute X^N ( mod P )
// HugeInt: must have copy constructor, operator=,
//     conversion from int, *, /, %, ==, and !=
//     Assumes that P is not zero and Power( 0, 0, P ) is 0

template <class HugeInt>
HugeInt
Power( const HugeInt & X, const HugeInt & N , const HugeInt & P )
{

    if( N == 0 )
        return 1;

    HugeInt Tmp = Power( ( X * X ) % P, N / 2, P );
    
    if( N % 2 != 0 )
        Tmp = ( Tmp * X ) % P;

    return Tmp;
}



// Cheap RandInt
// Obviously I should have used the Random class
// but some professors may want to cover this at the
// the same time as the rest of Chapter 7, so I avoided
// the reference to it.

extern "C" double drand48( );
int
RandInt( int Low, int High )
{
    return int( drand48( ) * ( High - Low + 1 ) + Low );
}

// Prime routine
// If Witness does not return 1, N is definitely composite
// Do this by computing A^i ( mod N ) and looking for
// non-trivial square roots of 1 along the way
// HugeInt: must have copy constructor, operator=,
//     conversion from int, *, /, -, %, ==, and !=

template <class HugeInt>
HugeInt
Witness( const HugeInt & A, const HugeInt & i, const HugeInt & N )
{
    if( i == 0 )
        return 1;

    HugeInt X = Witness( A, i / 2, N );
    if( X == 0 )    // If N is recursively composite, stop
        return 0;

    // N is not prime if we find a non-trivial square root of 1
    HugeInt Y  = ( X * X ) % N;
    if( Y == 1 && X != 1 && X != N - 1 )
        return 0;

    if( i % 2 )
        Y = ( A * Y ) % N;

    return Y;
}

// Make NumTrials call to Witness to check if N is prime

template <class HugeInt>
int
IsPrime( const HugeInt & N )
{
    const int NumTrials = 5;

    for( int Counter = 0; Counter < NumTrials; Counter++ )
        if( Witness( RandInt( 2, N - 2 ), N - 1, N ) != 1 )
            return 0;

    return 1;
}

template <class HugeInt>
HugeInt
Gcd( const HugeInt & A, const HugeInt & B )
{
    if( B == 0 )
        return A;
    else
        return Gcd( B, A % B );
}

// Given A and B, assume Gcd( A, B ) = 1 
// Find X and Y such that A X + B Y = 1
// HugeInt: must have copy constructor,
//     zero-parameter constructor, operator=,
//     conversion from int, *, /, +, -, %, ==, and >

template <class HugeInt>
void
FullGcd( const HugeInt & A, const HugeInt & B,
               HugeInt & X, HugeInt & Y )
{
    HugeInt X1, Y1;

    if( B == 0 )
    {
        X = 1;       // If A != 1, there is no Inverse
        Y = 0;       // We omit this check
    }
    else
    {
        FullGcd( B, A % B, X1, Y1 );
        X = Y1;
        Y = X1 - ( A / B ) * Y1;
    }
}

// Solve A X == 1 ( mod N )
// Assume that Gcd( A, N ) = 1

template <class HugeInt>
HugeInt
Inverse( const HugeInt & A, const HugeInt & N )
{
    HugeInt X, Y;

    FullGcd( A, N, X, Y );
    return X > 0 ? X : X + N;
}



// Simple main for testing purposes

const long A = 29;

main( )
{
    long X, N;

    for( N = 100; N < 10000; N++ )
    {
        if( N % A == 0 )
            continue;
        X = Inverse( A, N );
        if( X < 0 || ( A * X ) % N != 1 )
            cout << "Failed for " << N << '\n';
    }
    cout << "Inverse is " << Inverse( 13379L, 26460L ) << '\n';
    cout << "Gcd is " << Gcd( 123456789L, 987654321L ) << '\n';

    for( int i = 101; i <= 201; i+=2 )
        cout << i << "(" << IsPrime(i) << ")\n";

    cout << "Pow1 " << Power( 10237L, 13379L, 26797L ) << '\n';
    cout << "Pow2 " << Power( 8422L, 11099L, 26797L ) << '\n';

    return 0;
}
