#include "AATree.h"

// AATree constructor

template <class Etype>
AATree<Etype>::AATree( )
{
    NullNode = new BinaryNode<Etype>( );
    NullNode->Left = NullNode->Right = NullNode;
    NullNode->Level = 0;
    Root = NullNode;
}

// Usual rotation routines

template <class Etype>
BinaryNode<Etype> *
RotateWithLeftChild( BinaryNode<Etype> *K2 )
{
    BinaryNode<Etype> *K1 = K2->Left;
    K2->Left = K1->Right;
    K1->Right = K2;
    return K1;
}

template <class Etype>
BinaryNode<Etype> *
RotateWithRightChild( BinaryNode<Etype> *K1 )
{
    BinaryNode<Etype> *K2 = K1->Right;
    K1->Right = K2->Left;
    K2->Left = K1;
    return K2;
}

// Skew and Split: AA-tree primitives

template <class Etype>
void
AATree<Etype>::Skew( BinaryNode<Etype> * & T )
{
    if( T->Left->Level == T->Level )
        T = RotateWithLeftChild( T );
}

template <class Etype>
void
AATree<Etype>::Split( BinaryNode<Etype> * & T )
{
    if( T->Right->Right->Level == T->Level )
    {
        T = RotateWithRightChild( T );
        T->Level++;
    }
}

// Insert item X into AA-tree rooted at T
// Return true unless X is a duplicate

template <class Etype>
int
AATree<Etype>::Insert( const Etype & X, BinaryNode<Etype> * & T )
{
    if( T == NullNode )
    {
        T = new BinaryNode<Etype>( X, NullNode, NullNode );
        return 1;
    }
    else if( X < T->Element )
        Insert( X, T->Left );
    else if( T->Element < X )
        Insert( X, T->Right );
    else
        return 0;

    Skew( T );
    Split( T );
    return 1;
}

// Remove X from AA-tree rooted at T
// Return true unless X was not found

template <class Etype>
int
AATree<Etype>::Remove( const Etype & X, BinaryNode<Etype> * & T )
{
    static int ItemFound = 0;
    static BinaryNode<Etype> *DeletePtr;
    static BinaryNode<Etype> *LastPtr;

    if( T != NullNode )
    {
        // Step 1: Search down the tree and set LastPtr and DeletePtr
        LastPtr = T;
        if( X < T->Element )
            Remove( X, T->Left );
        else
        {
            DeletePtr = T;
            Remove( X, T->Right );
        }

        // Step 2: If at the bottom of the tree and
        //         X is present, we remove it

        if( T == LastPtr )
        {
            if( DeletePtr != NullNode && X == DeletePtr->Element )
            {
                DeletePtr->Element = T->Element;
                DeletePtr = NullNode;
                T = T->Right;
                delete LastPtr;
                ItemFound = 1;
            }
            else
                ItemFound = 0;
        }

        // Step 3: Otherwise, we are not at the bottom; rebalance
        else
            if( T->Left->Level < T->Level - 1 ||
                T->Right->Level < T->Level - 1 )
            {
                if( T->Right->Level > --T->Level )
                    T->Right->Level = T->Level;

                Skew( T );
                Skew( T->Right );
                Skew( T->Right->Right );
                Split( T );
                Split( T->Right );
            }
    }

    return ItemFound;
}

// All this stuff is the same as the binary search tree.
// This is what would have been saved by using inheritance.

template <class Etype>
const Etype &
AATree<Etype>::Find( const Etype & X )
{
    NullNode->Element = X;
    Current = Root;

    while( Current->Element != X )
        Current = X < Current->Element ? Current->Left : Current->Right;

    return Current->Element;
}

template <class Etype>
const Etype &
AATree<Etype>::FindMin( ) const
{
    BinaryNode<Etype> *Ptr = Root;
    while( Ptr->Left != NullNode )
        Ptr = Ptr->Left;

    return Ptr->Element;
}


template <class Etype>
const Etype &
AATree<Etype>::FindMax( ) const
{
    BinaryNode<Etype> *Ptr = Root;
    while( Ptr->Right != NullNode )
        Ptr = Ptr->Right;

    return Ptr->Element;
}

template <class Etype>
void
AATree<Etype>::FreeTree( BinaryNode<Etype> * T )
{
    if( T != NullNode )
    {
        FreeTree( T->Left );
        FreeTree( T->Right );
        delete T;
    }
}
