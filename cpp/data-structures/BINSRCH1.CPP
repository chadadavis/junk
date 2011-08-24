#include "Vector.h"

// BinarySearch: Return position of X in array A of N elements
//    Return last position examined; user can check
//    if item is found or not.
// Etype: must have operator< and operator== and all
//    properties needed for Vector

template <class Etype>
int
BinarySearch( const Vector<Etype> & A, const Etype & X, int N )
{
	int Low = 0, High = N - 1, Mid;

	while( Low < High )
	{
		Mid = ( Low + High ) / 2;
		if( A[ Mid ] < X )
			Low = Mid + 1;
		else
			High = Mid;
	}
	return Low;
}
