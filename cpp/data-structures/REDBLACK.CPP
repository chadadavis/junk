#include "RedBlack.h"

// Constructor for RedBlackTree

template <class Etype>
RedBlackTree<Etype>::RedBlackTree( const Etype & NegInf )
{
    NullNode = new BinaryNode<Etype>;
    Header = new BinaryNode<Etype>( NegInf );

    NullNode->Left = NullNode->Right =
            Header->Left = Header->Right = NullNode;
}

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

// Routine to choose and perform 1 of 4 types of rotations
// Sorry about that mess in the test -- I needed to compress it

template <class Etype>
BinaryNode<Etype> *
Rotate( const Etype & Item, BinaryNode<Etype> *Parent )
{
    if( Item < Parent->Element )
    {
        if( Item < Parent->Left->Element )
            return Parent->Left = RotateWithLeftChild( Parent->Left );
        else
            return Parent->Left = RotateWithRightChild( Parent->Left );
    }
    else
    {
        if( Item < Parent->Right->Element )
            return Parent->Right = RotateWithLeftChild( Parent->Right );
        else
            return Parent->Right = RotateWithRightChild( Parent->Right );
    }
}


// Private routine that is called during an insertion
// if a node has two red children

template <class Etype>
void
RedBlackTree<Etype>::HandleReorient( const Etype & Item )
{
        // Do the color flip
    Current->Color = Red;
    Current->Left->Color = Black;
    Current->Right->Color = Black;

    if( Parent->Color == Red )     // Have to rotate
    {
        Grand->Color = Red;
        if( ( Item < Grand->Element ) != ( Item < Parent->Element ) )
            Parent = Rotate( Item, Grand );  // Start double rotation
        Current = Rotate( Item, Great );
        Current->Color = Black;
    }
    Header->Right->Color = Black;    // Make root black
}

// Add X into T; return 0 if duplicate; otherwise return true

template <class Etype>
int
RedBlackTree<Etype>::Insert( const Etype & X )
{
    Current = Parent = Grand = Header;
    NullNode->Element = X;
    while( Current->Element != X )
    {
        Great = Grand; Grand = Parent; Parent = Current;
        Current = X < Current->Element ?
                Current->Left : Current->Right;

        if( Current->Left->Color == Red && Current->Right->Color == Red )
            HandleReorient( X );
    }

        // Do not add a duplicate
    if( Current != NullNode )
        return 0;
    Current = new BinaryNode<Etype>( X, NullNode, NullNode );

        // Attach to parent
    if( X < Parent->Element )
        Parent->Left = Current;
    else
        Parent->Right = Current;
    HandleReorient( X );
    return 1;
}

// Return element that matches X in a RedBlackTree
// WasFound should test if Current is the NullNode sentinel

template <class Etype>
const Etype &
RedBlackTree<Etype>::Find( const Etype & X )
{
    NullNode->Element = X;
    Current = Header->Right;

    while( Current->Element != X )
        Current = X < Current->Element ? Current->Left : Current->Right;

    return Current->Element;
}


// Return minimum (or maximum) element in RedBlackTree;
// -Infinity sentinel return if tree is empty

template <class Etype>
const Etype &
RedBlackTree<Etype>::FindMin( ) const
{
    BinaryNode<Etype> *Ptr = Header->Right;
    while( Ptr->Left != NullNode )
        Ptr = Ptr->Left;

    return Ptr->Element;
}


template <class Etype>
const Etype &
RedBlackTree<Etype>::FindMax( ) const
{
    BinaryNode<Etype> *Ptr = Header->Right;
    while( Ptr->Right != NullNode )
        Ptr = Ptr->Right;

    return Ptr->Element;
}

// Call delete for all nodes in RedBlackTree rooted at T

template <class Etype>
void
RedBlackTree<Etype>::FreeTree( BinaryNode<Etype> * T )
{
    if( T != NullNode )
    {
        FreeTree( T->Left );
        FreeTree( T->Right );
        delete T;
    }
}
