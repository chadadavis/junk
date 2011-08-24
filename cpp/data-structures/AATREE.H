// AATree class interface
//
// Etype: must have zero-parameter and copy constructor,
//     and must have operator< and operator=
// CONSTRUCTION: with (a) no initializer
// All copying of AATree objects is DISALLOWED
//
// ******************PUBLIC OPERATIONS*********************
// int Insert( Etype X )  --> Insert X
// int Remove( Etype X )  --> Remove X
// Etype Find( Etype X )  --> Return item that matches X
// int WasFound( )        --> Return 1 if last Find succeeded
// int IsFound( Etype X ) --> Return 1 if X would be found
// Etype FindMin( )       --> Return smallest item
// Etype FindMax( )       --> Return largest item
// int IsEmpty( )         --> Return 1 if empty; else return 0
// void MakeEmpty( )      --> Remove all items
// ******************ERRORS********************************
// Predefined exception is propogated if new fails
// Item in sentinel node returned on various degenerate conditions

#ifndef __AATree
#define __AATree

#include "BinNode.h"
#include <stdlib.h>

#define Level BalancingInfo

template <class Etype>
class AATree
{
  public:
    AATree( );
    ~AATree( ) { FreeTree( Root );  delete NullNode; }

    // Add X into the tree. If X already present, do nothing.
    // Return true if insertion was successful
    int Insert( const Etype & X )
        { return Insert( X, Root ); }

    // Remove X from tree. Return 1 if X was found; 0 otherwise
    int Remove( const Etype & X )
        { return Remove( X, Root ); }

    // Return minimum item in tree. If tree is empty, return undefined
    const Etype & FindMin( ) const;

    // Return maximum item in tree. If tree is empty, return undefined
    const Etype & FindMax( ) const;

    // Return item X in tree. If X is not found, return ItemNotFound.
    // Result can be checked by calling WasFound.
    const Etype & Find( const Etype & X );

    int WasFound( ) const
        { return Current != NullNode; }

    // MakeEmpty tree, and test if tree is empty.
    void MakeEmpty( )
        { FreeTree( Root ); Root = NullNode; }
    int IsEmpty( ) const
        { return Root == NullNode; }
  private:
    BinaryNode<Etype> *Root;
    BinaryNode<Etype> *NullNode;

    // Result of last Find
    BinaryNode<Etype> *Current;

    void Skew ( BinaryNode<Etype> * & T );
    void Split( BinaryNode<Etype> * & T );
    
    void FreeTree( BinaryNode<Etype> *T );
    int Insert( const Etype & X, BinaryNode<Etype> * & T );
    int Remove( const Etype & X, BinaryNode<Etype> * & T );

    // Disable operator=
    const AATree & operator=( const AATree & );
};
#endif
