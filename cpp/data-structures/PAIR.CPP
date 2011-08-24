#include "Pair.h"
#include "Vector.h"
#include <iostream.h>
#include <stdlib.h>
#include "Exception.h"

// Insert X into pairing heap
// Return a pointer to the inserted node

template <class Etype>
PairNode<Etype> *
PairHeap<Etype>::Insert( const Etype & X )
{
    PairNode<Etype> *NewNode = new PairNode<Etype>( X );

    CurrentSize++;

    if( Root == NULL )
        Root = NewNode;
    else
        CompareAndLink( Root, NewNode );

    return NewNode;
}

// Return minimum item in pairing heap

template <class Etype>
const Etype &
PairHeap<Etype>::FindMin( ) const
{
    EXCEPTION( IsEmpty( ), "Pairing heap is empty" );
    return Root->Element;
}

// Delete minimum item in pairing heap; place result in X

template <class Etype>
void
PairHeap<Etype>::DeleteMin( Etype & X )
{
    EXCEPTION( IsEmpty( ), "Pairing heap is empty" );
    PairNode<Etype> *OldRoot = Root;

    X = Root->Element;
    if( Root->LeftChild == NULL )
        Root = NULL;
    else
        Root = CombineSiblings( Root->LeftChild );

    delete OldRoot;
    CurrentSize--;
}

// Delete minimum item in pairing heap

template <class Etype>
void
PairHeap<Etype>::DeleteMin( )
{
    static Etype Ignored;    // To avoid repeated constructions
    DeleteMin( Ignored );
}

// This is the basic operation to maintain order.
// Links First and Second together to satisfy heap order.
// First is set to the resulting tree.
// First is assumed not NULL
// First->NextSibling MUST be NULL on entry.

template <class Etype>
void
PairHeap<Etype>::CompareAndLink( PairNode<Etype> * & First, PairNode<Etype> *Second )
{
    if( Second == NULL )
        return;

    if( Second->Element < First->Element )
    {
        // Attach First as leftmost child of Second
        Second->Prev = First->Prev;
        First->Prev = Second;
        First->NextSibling = Second->LeftChild;
        if( First->NextSibling != NULL )
            First->NextSibling->Prev = First;
        Second->LeftChild = First;
        First = Second;   // Second becomes new root
    }
    else
    {
        // Attach Second as leftmost child of First
        Second->Prev = First;
        First->NextSibling = Second->NextSibling;
        if( First->NextSibling != NULL )
            First->NextSibling->Prev = First;
        Second->NextSibling = First->LeftChild;
        if( Second->NextSibling != NULL )
            Second->NextSibling->Prev = Second;
        First->LeftChild = Second;
    }
}

// Change value of key stored at node
// pointed at by P to NewVal;
// NewVal must be smaller than originally stored value

template <class Etype>
void
PairHeap<Etype>::DecreaseKey( PairNode<Etype> *P, const Etype & NewVal )
{
    if( P->Element < NewVal )
        cerr << "DecreaseKey called with larger value!" << endl;
    else
    {
        P->Element = NewVal;
        if( P != Root )
        {
            if( P->NextSibling != NULL )
                P->NextSibling->Prev = P->Prev;
            if( P->Prev->LeftChild == P )
                P->Prev->LeftChild = P->NextSibling;
            else
                P->Prev->NextSibling = P->NextSibling;

            P->NextSibling = NULL;
            CompareAndLink( Root, P );
        }
    }
}

// CombineSiblings assumes that FirstSibling is not NULL

template <class Etype>
PairNode<Etype> *
PairHeap<Etype>::CombineSiblings( PairNode<Etype> *FirstSibling )
{
    if( FirstSibling->NextSibling == NULL )
        return FirstSibling;

        // Allocate the array
    Vector<PairNode<Etype> *> TreeArray( CurrentSize );

        // Store the subtrees in an array
    int NumSiblings;
    for( NumSiblings = 0; FirstSibling != NULL; NumSiblings++ )
    {
        TreeArray[ NumSiblings ] = FirstSibling;
        FirstSibling->Prev->NextSibling = NULL; // break links
        FirstSibling = FirstSibling->NextSibling;
    }
    TreeArray[ NumSiblings ] = NULL;

    // Combine the subtrees two at a time, going left to right
    int i;
    for( i = 0; i+1 < NumSiblings; i+=2 )
        CompareAndLink( TreeArray[ i ], TreeArray[ i + 1 ] );

    int j = i - 2;

    // j has the result of the last CompareAndLink.
    // If an odd number of trees, get the last one

    if( j == NumSiblings - 3 )
        CompareAndLink( TreeArray[ j ], TreeArray[ j + 2 ] );

    // Now go right to left, merging last tree with
    // next to last. The result becomes the new last.

    for( ; j >= 2; j -= 2 )
        CompareAndLink( TreeArray[ j - 2 ], TreeArray[ j ] );

    PairNode<Etype> *Result = TreeArray[ 0 ];
    return Result;
}

// Delete nodes in the tree

template <class Etype>
void
PairHeap<Etype>::FreeTree( PairNode<Etype> * T )
{
    if( T != NULL )
    {
        FreeTree( T->NextSibling );
        FreeTree( T->LeftChild );
        delete T;
    }
}
