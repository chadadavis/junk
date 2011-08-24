// HashTable class interface
//
// Etype: must have zero-parameter and copy constructor,
//     operator= and operator!=
// CONSTRUCTION: with (a) no initializer;
// Copy constructon of HashTable objects is DISALLOWED
// Deep copy is supported
//
// ******************PUBLIC OPERATIONS*********************
// int Insert( Etype X )  --> Insert X
// int Remove( Etype X )  --> Remove X
// Etype Find( Etype X )  --> Return item that matches X
// int WasFound( )        --> Return 1 if last Find succeeded
// int IsFound( Etype X ) --> Return 1 if X would be found
// int IsEmpty( )         --> Return 1 if empty; else return 0
// int IsFull( )          --> Return 1 if full; else return 0
// void MakeEmpty( )      --> Remove all items
// ******************ERRORS********************************
// Predefined exception is propogated if new fails

#ifndef __HashTable
#define __HashTable

#include "AbsHash.h"

template <class Etype>
class HashTable : public AbsHTable<Etype>
{
  public:
    enum KindOfEntry { Active, Empty, Deleted };

    HashTable( );
    ~HashTable( ) { delete [ ] Array; }

    const HashTable & operator=( const HashTable & Rhs );

    int Insert( const Etype & X );
    int Remove( const Etype & X );         // Return 1 if found, 0 if not
    const Etype & Find( const Etype & X ); // Return item in table
    int IsFound( const Etype & X ) const;  // Return 1 if found
    int WasFound( ) const;                 // Return 1 if last find ok
    int IsEmpty( ) const;
    int IsFull( ) const { return 0; }      // Return 1 if full
    void MakeEmpty( );
  private:
    struct HashEntry
    {
        Etype Element;         // The item
        KindOfEntry Info;      // Active, empty, or deleted

        HashEntry( ) : Info( HashTable<Etype>::Empty ) { }
        HashEntry( const Etype & E, KindOfEntry i = Empty ) :
            Element( E ), Info( i ) { }
    };

    enum { DefaultSize = 11 };

    int ArraySize;       // The size of this array
    int CurrentSize;     // The number of non-empty items
    int LastFindOK;      // True of last search was successful
    HashEntry *Array;    // The array of elements

        // Some internal routines
    void AllocateArray( );
    unsigned int FindPos( const Etype & X ) const;
};
#endif
