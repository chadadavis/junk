#include "Pair.h"
#include <iostream.h>

#ifdef __GNUC__
	#include "Pair.cpp"
#endif

main( )
{
	PairHeap<int> P;

	for( int i = 0; i < 20; i+=2 )
		P.Insert( i );
	for( i = 19; i >= 1; i-= 2 )
		P.DecreaseKey( P.Insert( i ), i-8 );

	while( !P.IsEmpty( ) )
	{
		int X;
		P.DeleteMin( X );
		cout << X << endl;
	}

	return 0;
}
	
