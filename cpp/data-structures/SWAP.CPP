// Illustration of call by reference vs. call with pointers.

#include <iostream.h>

// Does not work
void
SwapWrong( int A, int B )
{
    int Tmp = A;
    A = B;
    B = Tmp;
}

// C Style -- using pointers
void
SwapPtr( int *A, int *B )
{
    int Tmp = *A;
    *A = *B;
    *B = Tmp;
}

// C++ style -- using references
void
SwapRef( int & A, int & B )
{
    int Tmp = A;
    A = B;
    B = Tmp;
}

// Simple program to test various Swap routines
main( )
{
    int X = 5;
    int Y = 7;

    SwapWrong( X, Y );
    cout << "X=" << X << " Y=" << Y << '\n';
    SwapPtr( &X, &Y );
    cout << "X=" << X << " Y=" << Y << '\n';
    SwapRef( X, Y );
    cout << "X=" << X << " Y=" << Y << '\n';

    return 0;
}
    
