#include <iostream.h>
#include "List.h"

#ifdef __GNUC__
	#include "List.cpp"
// g++ hack needed to see template instantiation.
// Go figure...
ostream & operator<<( ostream &, const List<int> & );
#endif


main( void )
{
	List<int> L;
	ListItr<int> Ptr( L );
	List<int> L1;
	SortListItr<int> Ptr2( L1 );

	cout << L;
	for( int i = 0; i < 10; i++ )
		Ptr.Insert( i );
	cout << L;
	Ptr.Find( 5 );
	Ptr.Insert( 12 );
	Ptr.Remove( 99 );
	cout << L;
	for( Ptr.First( ); +Ptr; ++Ptr )
		cout << Ptr( ) << endl;
	Ptr++;
	Ptr( );

	List<int> L2;
	L2 = L;
	cout << L2;
	L.MakeEmpty( );
	cout << L << L2;

	for( i = 10; i != 0; i = (i+3)% 10 )
		Ptr2.Insert( i );
	cout << L1;
	return 0;
}
