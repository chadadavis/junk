// PairHeap class interface
// Etype: must have zero-parameter constructor,
//     copy constructor, operator=, and operator<
// CONSTRUCTION: with (a) Etype representing negative infinity
// All copying of PairHeap objects is DISALLOWED
//
// ******************PUBLIC OPERATIONS*************
// PairNode *Insert( Etype X ) --> Insert X; return position
// void DeleteMin( )      --> Remove smallest item
// Etype FindMin( )       --> Return smallest item
// void DeleteMin( Etype & X ) --> Remove smallest item, put it in X
// int IsEmpty( )         --> Return 1 if empty; else return 0
// int IsFull( )          --> Return 1 if full; else reutrn 0
// void MakeEmpty( )      --> Remove all items
// void DecreaseKey( PairNode *P, Etype NewValue )
//                        --> Lower value of item at P
// ******************ERRORS************************
// Predefined exception is propagated if new fails
// EXCEPTION is called for FindMin or DeleteMin when empty
// Error message for DecreaseKey with higher value

#ifndef __PairHeap
#define __PairHeap

#include <stdlib.h>

template <class Etype>
class PairNode
{
    Etype Element;
    PairNode *LeftChild;
    PairNode *NextSibling;
    PairNode *Prev;

    friend class PairHeap<Etype>;

  public:
    PairNode( const Etype & E ) : Element( E ), LeftChild( NULL ),
            NextSibling( NULL ), Prev( NULL ) { }
};

template <class Etype>
class PairHeap
{
  public:
    PairHeap( ) : Root( NULL ), CurrentSize( 0 ) { }
    ~PairHeap( ) { FreeTree( Root ); }

        // Add an item maintaining heap order; return position
    PairNode<Etype> *Insert( const Etype & X );

        // Return minimum item in heap
    const Etype & FindMin( ) const;

        // Delete minimum item in heap
    void DeleteMin( );
    void DeleteMin( Etype & X );

        // Lower the value of a key in node given by P
    void DecreaseKey( PairNode<Etype> *P, const Etype & NewVal );

        // The usual suspects
    int IsEmpty( ) const { return CurrentSize == 0; }
    int IsFull( ) const  { return 0; }
    void MakeEmpty( )    { CurrentSize = 0; FreeTree( Root ); }
  private:
    PairNode<Etype> *Root;
    int CurrentSize;     // Number of elements currently stored

    PairHeap( const PairHeap & ); // Disable copy constructor
    const PairHeap & operator=( const PairHeap & Rhs ); // Disable op=

        // Internal routines
    void CompareAndLink( PairNode<Etype> * & First, PairNode<Etype> * Second );
    PairNode<Etype> *CombineSiblings( PairNode<Etype> *FirstSibling );
    void FreeTree( PairNode<Etype> *T );

};
#endif
