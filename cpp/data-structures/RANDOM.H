// Random package that calls the UNIX routine
// random( ) and srandom( ) if it can, but uses
// a linear congruential generator otherwise.

#ifndef __RANDOM
#define __RANDOM

#ifdef unix
    extern "C" void srandom( unsigned );
    extern "C" long random( );
#endif


// Random class interface
//
// CONSTRUCTION: with (a) no initializer or (b) an integer
//     that specifies the initial state of the generator
//
// ******************PUBLIC OPERATIONS*********************
//     Return a random number according to some distribution
// unsigned long RandomLong( )          --> Uniform, 1 to 2^31-1
// double RandomReal( )                 --> Uniform, 0..1
// long RandLong( long Low, long High ) --> Uniform Low..High
// unsigned Poisson( double ExpectedVal )-> Poisson
// double NegExp( double ExpectedVal )  --> Negative exponential


class Random
{
  public:
    Random( unsigned long InitVal = 1 ) : Seed ( InitVal )
    {
#ifdef unix
    srandom( InitVal );
#endif
    }


        // Uniform distributions
    unsigned long RandomLong( );   // 1 to 2^31 - 1
    double RandomReal( );          // >0.0 to <1.0
    long RandLong( long Low, long High );

        // Non-uniform distribution
    unsigned Poisson( double ExpectedVal );
    double NegExp( double ExpectedVal );
  private:
    unsigned long Seed;
};
#endif
