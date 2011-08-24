// RedBlackTree class interface
//
// Etype: must have zero-parameter and copy constructor,
//     and must have operator<, operator!=, and operator=
// CONSTRUCTION: with (a) Negative infinity sentinel
// All copying of RedBlackTree objects is DISALLOWED
//
// ******************PUBLIC OPERATIONS*********************
// int Insert( Etype X )  --> Insert X
// Etype Find( Etype X )  --> Return item that matches X
// int WasFound( )        --> Return 1 if last Find succeeded
// int IsFound( Etype X ) --> Return 1 if X would be found
// Etype FindMin( )       --> Return smallest item
// Etype FindMax( )       --> Return largest item
// int IsEmpty( )         --> Return 1 if empty; else return 0
// void MakeEmpty( )      --> Remove all items
// ******************ERRORS********************************
// Predefined exception is propogated if new fails
// Item in NullNode returned on various degenerate conditions


#ifndef __RedBlack
#define __RedBlack
#include "BinNode.h"
#include <stdlib.h>

#define Color BalancingInfo

template <class Etype>
class RedBlackTree
{
  public:
    RedBlackTree( const Etype & NegInf );
    ~RedBlackTree( ) { FreeTree( Header );  delete NullNode; }

    // Add X into the tree. If X already present, do nothing.
    // Return true if insertion was successful
    int Insert( const Etype & X );

    // Return minimum item in tree. If tree is empty, return undefined
    const Etype & FindMin( ) const;

    // Return maximum item in tree. If tree is empty, return undefined
    const Etype & FindMax( ) const;

    // Return item X in tree. If X is not found, return item in NullNode
    // Result can be checked by calling WasFound.
    const Etype & Find( const Etype & X );

    int WasFound( ) const
        { return Current != NullNode; }

    // MakeEmpty tree, and test if tree is empty.
    void MakeEmpty( )
        { FreeTree( Header->Right ); Header->Right = NullNode; }

    int IsEmpty( ) const
        { return Header->Right == NullNode; }
  private:
    enum { Black = 1, Red = 0 };  // Black==1 for default new

    BinaryNode<Etype> *NullNode;
    BinaryNode<Etype> *Header;

    // Globals used to traverse down the tree in an insert
    BinaryNode<Etype> *Current;
    BinaryNode<Etype> *Parent;
    BinaryNode<Etype> *Grand;
    BinaryNode<Etype> *Great;

    void HandleReorient( const Etype & Item );
    void FreeTree( BinaryNode<Etype> *T );

    // Remove X from the tree. Not currently provided
    int Remove( const Etype & X );

    // Disable operator=
    const RedBlackTree & operator=( const RedBlackTree & );
};
#endif
