// SplayTree class interface
//
// Etype: must have zero-parameter and copy constructor
//     and must have operator< and operator==
// CONSTRUCTION: with (a) no initializer
// All copying of SplayTree objects is DISALLOWED
//
// ******************PUBLIC OPERATIONS****************
// int Insert( Etype X )  --> Insert X
// int Remove( Etype X )  --> Remove X
// Etype GetRoot( )       --> Return item in root
// int IsFound( Etype X ) --> Return 1 if X would be found
// int IsEmpty( )         --> Return 1 if empty; else return 0
// void MakeEmpty( )      --> Remove all items
// ******************ERRORS***************************
// Predefined exception is propagated if new fails
// Item in NulNode return for Find on empty tree


#ifndef __SPLAY
#define __SPLAY
#include <stdlib.h>

template <class Etype>
struct BinaryNode
{
    Etype Element;       // The data in the node
    BinaryNode *Left;    // Left child
    BinaryNode *Right;   // Right child

    BinaryNode( ) : Left( NULL ), Right( NULL ) { }
    BinaryNode( const Etype & E ) :
        Element( E ), Left( NULL ), Right( NULL ) { }
};

template <class Etype>
class SplayTree
{
  public:
    SplayTree( );
    ~SplayTree( ) { FreeTree( Root );  delete NilNode; }

    // Add X into the tree. If X already present, do nothing.
    // Return true if insertion was successful
    int Insert( const Etype & X );

    // Remove X from tree. Return 1 if X was found; 0 otherwise
    int Remove( const Etype & X );

    // If X is found, it will be at the root.
    // Thus IsFound returns success or failure.
    // GetRoot can be used to see what is there
    int IsFound( const Etype & X )
        { Splay( X, Root );
          return Root != NilNode && Root->Element == X; }
    const Etype & GetRoot( ) const
        { return Root->Element; }

    // Make empty tree, and test if tree is empty.
    void MakeEmpty( )
        { FreeTree( Root ); Root = NilNode; }
    int IsEmpty( ) const
        { return Root == NilNode; }
  private:
    BinaryNode<Etype> *NilNode;
    BinaryNode<Etype> *Root;

    void FreeTree( BinaryNode<Etype> *T );
    void Splay( const Etype & X, BinaryNode<Etype> * & T );

        // Disable copying
    SplayTree( const SplayTree & );
    const SplayTree & operator=( const SplayTree & );
};
#endif
