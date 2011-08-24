#include "Ordered.h"

// Internal Insert routine for OrderedSearchTree:
// Add X into tree rooted at T and return true
// unless X is a duplicate

template <class Etype>
int
OrderedSearchTree<Etype>::Insert( const Etype & X, BinaryNode<Etype> * & T )
{
    if( T == NULL )
        return ( T = new BinaryNode<Etype>( X ) ) != NULL;
    else if( X < T->Element )
        return Insert( X, T->Left ) ? ++T->Size : 0;
    else if( T->Element < X )
        return Insert( X, T->Right ) ? ++T->Size : 0;
    else  // Duplicate -- do nothing
        return 0;
}

// Internal Remove for OrderedSearchTree:
// Remove X from tree rooted at T; return true
// if X was found and false otherwise

template <class Etype>
int
OrderedSearchTree<Etype>::Remove( const Etype & X, BinaryNode<Etype> * & T )
{
    BinaryNode<Etype> *Tmp = NULL;

    if( T == NULL )
        return 0;
    else
    if( X < T->Element )   // Go left
        return Remove( X, T->Left ) ? T->Size-- : 0;
    else
    if( T->Element < X )   // Go right
        return Remove( X, T->Right ) ? T->Size-- : 0;
    else
    if( T->Left != NULL && T->Right != NULL )  // Two children
    {
        Tmp = (BinaryNode<Etype> *) FindMin( T->Right );
        T->Element = Tmp->Element;
        T->Size--;
        return RemoveMin( T->Right );       // Remove minimum
    }

        // One or zero children
    Tmp = T;
    T = ( T->Left != NULL ) ? T->Left : T->Right;
    delete Tmp;
    return 1;
}

// Internal RemoveMin for OrderedSearchTree
// Remove minimum item from tree rooted at T and
// return true if tree was not empty prior to removal

template <class Etype>
int
OrderedSearchTree<Etype>::RemoveMin( BinaryNode<Etype> * & T )
{
    if( T == NULL )
        return 0;
    else if( T->Left != NULL )
        return RemoveMin( T->Left ) ? T->Size-- : 0;

    BinaryNode<Etype> *Tmp = T;
    T = T->Right;
    delete Tmp;
    return 1;
}

// Internal FindKth routine for OrderedSearchTree:
// Return pointer to node containing Kth smallest item
// in tree rooted at T or NULL if K is out of range

template <class Etype>
const BinaryNode<Etype> *
OrderedSearchTree<Etype>::FindKth( int K, const BinaryNode<Etype> * T ) const
{
    if( T == NULL )
        return NULL;

    int LeftSize = T->Left ? T->Left->Size : 0;

    if( K <= LeftSize )
        return FindKth( K, T->Left );
    if( K == LeftSize + 1 )
        return T;
    return FindKth(  K - LeftSize - 1, T->Right );
}
