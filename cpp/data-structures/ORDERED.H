// OrderedSearchTree class interface
//
// Etype: must have zero-parameter and copy constructor,
//     and must have operator<
// CONSTRUCTION: with (a) no initializer;
// All copying of OrderedSearchTree objects is DISALLOWED
//
// ******************PUBLIC OPERATIONS*********************
// int Insert( Etype X )  --> Insert X
// int Remove( Etype X )  --> Remove X
// Etype Find( Etype X )  --> Return item that matches X
// Etype FindKth( int K ) --> Return Kth smallest item
// int WasFound( )        --> Return 1 if last Find succeeded
// int IsFound( Etype X ) --> Return 1 if X would be found
// Etype FindMin( )       --> Return smallest item
// Etype FindMax( )       --> Return largest item
// int IsEmpty( )         --> Return 1 if empty; else return 0
// void MakeEmpty( )      --> Remove all items
// ******************ERRORS********************************
// Predefined exception is propogated if new fails
// ItemNotFound returned on various degenerate conditions

#ifndef __Ordered
#define __Ordered

#include "Bst.h"

template <class Etype>
class OrderedSearchTree : public SearchTree<Etype>
{
  public:
    OrderedSearchTree( ) { }


    // Return kth item in tree.
    // If X is not found, return ItemNotFound.
    // Result can be checked by calling WasFound.
    const Etype & FindKth( int K )
        { if( Root == NULL ) return ItemNotFound;
                      else return FindKth( K, Root )->Element; }

    // Add X into the tree. If X already present, do nothing.
    // Return true if insertion was successful
    int Insert( const Etype & X )
        { return Insert( X, Root ); }

    // Remove X from the tree. Return true if successful
    int Remove( const Etype & X )
        { return Remove( X, Root ); }

    // Remove minimum item from the tree. Return true if successful
    int RemoveMin( )
        { return RemoveMin( Root ); }

  private:
        // New routines
    int Insert( const Etype & X, BinaryNode<Etype> * & T );
    int Remove( const Etype & X, BinaryNode<Etype> * & T );
    int RemoveMin( BinaryNode<Etype> * & T );

    const BinaryNode<Etype> *FindKth( int K,
                                       const BinaryNode<Etype> * T ) const;
    int TreeSize( ) const { return Root ? Root->Size : 0; }
};

#endif
