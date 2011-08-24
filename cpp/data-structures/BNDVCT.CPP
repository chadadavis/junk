#include "BndVct.h"

    // g++ does not support separate compilation of templates
#ifdef __GNUC__
    #include "Vector.cpp"
#endif

template <class Etype>
Etype &
BoundedVector<Etype>::operator [ ] ( int Index )
{
    return Vector<Etype>::operator[ ] ( Index - Low );
}
    
template <class Etype>
const Etype &
BoundedVector<Etype>::operator [ ] ( int Index ) const
{
    return Vector<Etype>::operator[ ] ( Index - Low );
}
    

// Simple test program

main( )
{
    BoundedVector<double> V( 20, 39 );
    BoundedVector<double> W( 20, 39 );


    for( int i = 20; i < 40; i++)
        cout << V[ i ] << " ";
    cout << endl;
    for( i = 20; i < 40; i++ )
        V[ i ] = i;
    for( i = 20; i < 40; i++)
        cout << V[ i ] << " ";
    cout << endl;
    V.Double( );
    cout << V[50];
    cout << V[70];

    return 0;
}
