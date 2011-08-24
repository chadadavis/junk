#include "Splay.h"

// Constructor for SplayTree

template <class Etype>
SplayTree<Etype>::SplayTree( )
{
    NilNode = new BinaryNode<Etype>;
    NilNode->Left = NilNode->Right = NilNode;
    Root = NilNode;
}

// Insert X into splay tree
// Return faluse if X is a duplicate; true otherwise

template <class Etype>
int
SplayTree<Etype>::Insert( const Etype & X )
{
    static BinaryNode<Etype> *NewNode = NULL;

    if( NewNode == NULL )
        NewNode = new BinaryNode<Etype>( X );

    if( Root == NilNode )
    {
        NewNode->Left = NewNode->Right = NilNode;
        Root = NewNode;
    }
    else
    {
        Splay( X, Root );
        if( X < Root->Element )
        {
            NewNode->Left = Root->Left;
            NewNode->Right = Root;
            Root->Left = NilNode;
            Root = NewNode;
        }
        else
        if( Root->Element < X )
        {
            NewNode->Right = Root->Right;
            NewNode->Left = Root;
            Root->Right = NilNode;
            Root = NewNode;
        }
        else    // Already in the tree
            return 0;
    }

    NewNode = NULL;   // So next Insert will call new
    return 1;
}


// Remove X from splay tree
// Return true if X was found; false otherwise

template <class Etype>
int
SplayTree<Etype>::Remove( const Etype & X )
{
    BinaryNode<Etype> *NewTree;

    if( !IsFound( X ) )
        return 0;

        // If X is found, it will be at the root
    if( Root->Left == NilNode )
        NewTree = Root->Right;
    else
    {    
        // Find the maximum inthe left subtree
        // splay it to root; and then attach right child
        NewTree = Root->Left;
        Splay( X, NewTree );
        NewTree->Right = Root->Right;
    }
    delete Root;
    Root = NewTree;
    return 1;
}

// Top-down splay routine
// Last accessed node becomes the new root

template <class Etype>
void
SplayTree<Etype>::Splay( const Etype & X, BinaryNode<Etype> * & T )
{
    BinaryNode<Etype> *LeftTreeMax, *RightTreeMin, Header;

    Header.Left = Header.Right = NilNode;
    LeftTreeMax = RightTreeMin = &Header;

    // Copy X to NilNode to guarantee match
    NilNode->Element = X;

    for( ; ; )
        if( X < T->Element )
        {
            if( X < T->Left->Element )
                T = RotateWithLeftChild( T );
            if( T->Left == NilNode )
                break;
            // Link Right
            RightTreeMin->Left = T;
            RightTreeMin = T;
            T = T->Left;
        }
        else if( T->Element < X )
        {
            if( T->Right->Element < X )
                T = RotateWithRightChild( T );
            if( T->Right == NilNode )
                break;
            // Link Left
            LeftTreeMax->Right = T;
            LeftTreeMax = T;
            T = T->Right;
        }
        else
            break;

    LeftTreeMax->Right = T->Left;
    RightTreeMin->Left = T->Right;
    T->Left = Header.Right;
    T->Right = Header.Left;
}
            
// The usual suspects that have alrady been seen before

template <class Etype>
void
SplayTree<Etype>::FreeTree( BinaryNode<Etype> * T )
{
    if( T != NilNode )
    {
        FreeTree( T->Left );
        FreeTree( T->Right );
        delete T;
    }
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
