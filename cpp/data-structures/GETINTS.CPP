#include <iostream.h>
#include <stdlib.h>

// Read an unlimited number of ints.
// No attempts at error recovery
// Return a pointer to the data, and set ItemsRead.

int *
GetInts( int & ItemsRead )
{
    int ArraySize = 0;
    int InputVal;
    int *Array = 0;   // Initialize to NULL pointer
    
    ItemsRead = 0;
    cout << "Enter any number of integers: ";
    while( cin >> InputVal )
    {
        if( ItemsRead == ArraySize )
        {    // Array Doubling Code
            int *Original = Array;
            Array = new int[ ArraySize * 2 + 1 ];
            for( int i = 0; i < ArraySize; i++ )
                Array[ i ] = Original[ i ];
            delete [ ] Original;  // Safe if Original is NULL
            ArraySize = ArraySize * 2 + 1;
        }
        Array[ ItemsRead++ ] = InputVal;
    }
    return Array;
}

main( )
{
    int *Array;
    int NumItems;

    // The actual code
  try
  {
    Array = GetInts( NumItems ); 
    for( int i = 0; i < NumItems; i++ )
        cout << Array[ i ] << '\n';
  }
   // Exception handler; ... used because standard is evolving
  catch( ... )
  {
    cerr << "Out of memory!" << endl; exit( 1 );
  }
    return 0;
}
