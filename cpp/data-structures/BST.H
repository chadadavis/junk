// SearchTree class interface
//
// Etype: must have zero-parameter and copy constructor,
//     and must have operator<
// CONSTRUCTION: with (a) no initializer;
// All copying of SearchTree objects is DISALLOWED
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
// ItemNotFound returned on various degenerate conditions
//
// All routines are made virtual in case classes are derived
// This includes trivial routines because we might want to
// implement lazy deletion as a derived class


#ifndef __Bst
#define __Bst

#include "BinNode.h"

template <class Etype>
class SearchTree
{
  public:
    SearchTree( ) : LastFind( NULL ), Root( NULL ) { }
    virtual ~SearchTree( ) { FreeTree( Root ); }

    // Add X into the tree. If X already present, do nothing.
    // Return 1 if successful
    virtual int Insert( const Etype & X )
        { return Insert( X, Root ); }

    // Remove X from the tree. Return true if successful
    virtual int Remove( const Etype & X )
        { return Remove( X, Root ); }

    // Remove minimum item from the tree. Return true if successful
    virtual int RemoveMin( )
        { return RemoveMin( Root ); }

    // Return minimum item in tree. If tree is empty, return ItemNotFouud
    virtual const Etype & FindMin( ) const
        { const BinaryNode<Etype> *P = FindMin( Root );
              return P ? P->Element : ItemNotFound; }

    // Return maximum item in tree. If tree is empty, return ItemNotFund
    virtual const Etype & FindMax( ) const
        { const BinaryNode<Etype> *P = FindMax( Root );
              return P ? P->Element : ItemNotFound; }

    // Return item X in tree. If X is not found, return ItemNotFound.
    // Result can be checked by calling WasFound.
    virtual const Etype & Find( const Etype & X )
        { return ( LastFind = Find( X, Root ) ) ?
                      LastFind->Element : ItemNotFound; }

    // Return true if X is in tree.
    virtual int IsFound( const Etype & X )
        { return Find( X, Root ) != NULL; }

    // Return true if last call to Find was successful.
    virtual int WasFound( ) const
        { return LastFind != NULL; }

    // MakeEmpty tree, and test if tree is empty.
    virtual void MakeEmpty( )
        { FreeTree( Root ); Root = NULL; }
    virtual int IsEmpty( ) const
        { return Root == NULL; }

    virtual const BinaryNode<Etype> *GetRoot( ) const { return Root; }
  protected:
    BinaryNode<Etype> *Root;
    const BinaryNode<Etype> *LastFind;
    Etype ItemNotFound;  // Used for returns of not found

        // Disable all copying
    SearchTree( const SearchTree & );
    const SearchTree & operator=( const SearchTree & );

        // Simple recursive routine
    virtual void FreeTree( BinaryNode<Etype> *T );
    virtual int Insert( const Etype & X, BinaryNode<Etype> * & T );
    virtual int Remove( const Etype & X, BinaryNode<Etype> * & T );
    virtual int RemoveMin( BinaryNode<Etype> * & T );


    virtual const BinaryNode<Etype> *FindMin( const BinaryNode<Etype> *T ) const;
    virtual const BinaryNode<Etype> *FindMax( const BinaryNode<Etype> *T ) const;
    virtual const BinaryNode<Etype> *Find( const Etype &X,
                                       const BinaryNode<Etype> * T ) const;
};
#endif
