#include "Bst.h"

template <class Etype>
void
SearchTree<Etype>::FreeTree( BinaryNode<Etype> * T )
{
    if( T != NULL )
    {
        FreeTree( T->Left );
        FreeTree( T->Right );
        delete T;
    }
}
// Internal Insert routine for SearchTree:
// Add X into subtree rooted at T and return true
// if insert succeeds and false if X is a duplicate

template <class Etype>
int
SearchTree<Etype>::Insert( const Etype & X, BinaryNode<Etype> * & T )
{
    if( T == NULL )
    {
        T = new BinaryNode<Etype>( X );
        return 1;
    }
    else if( X < T->Element )
        return Insert( X, T->Left );
    else if( T->Element < X )
        return Insert( X, T->Right );
    // Don't insert duplicates
    return 0;
}

// Internal Remove routine for SearchTree:
// Remove X from tree rooted at T
// Return false only if X was not found

template <class Etype>
int
SearchTree<Etype>::Remove( const Etype & X, BinaryNode<Etype> * & T )
{
    BinaryNode<Etype> *Tmp;

    if( T == NULL )
        return 0;
    else
    if( X < T->Element )   // Go left
        return Remove( X, T->Left );
    else
    if( T->Element < X )   // Go right
        return Remove( X, T->Right );
    else
    if( T->Left != NULL && T->Right != NULL )  // Two children
    {
        // Copy from node to delete into node to keep; then delete
        Tmp = (BinaryNode<Etype> *) FindMin( T->Right );
        T->Element = Tmp->Element;
        return RemoveMin( T->Right );         // Remove minimum
    }
    // One or zero children
    Tmp = T;
    T = ( T->Left != NULL ) ? T->Left : T->Right;  // Reroot T
    delete Tmp;                                    // Delete old root
    return 1;
}

// Internal RemoveMin routine for SearchTree:
// Remove minimum item in tree rooted at T
// Return false only if tree is empty

template <class Etype>
int
SearchTree<Etype>::RemoveMin( BinaryNode<Etype> * & T )
{
    if( T == NULL )
        return 0;
    else if( T->Left != NULL )
        return RemoveMin( T->Left );

    BinaryNode<Etype> *Tmp = T;
    T = T->Right;
    delete Tmp;
    return 1;
}

// Internal FindMin and FindMax for SearchTree:
// Return pointer to node containing minimum/maximum
// or NULL if tree is empty

template <class Etype>
const BinaryNode<Etype> *
SearchTree<Etype>::FindMin( const BinaryNode<Etype> * T ) const
{
    if( T != NULL )
        while( T->Left != NULL )
            T = T->Left;

    return T;
}

template <class Etype>
const BinaryNode<Etype> *
SearchTree<Etype>::FindMax( const BinaryNode<Etype> * T ) const
{
    if( T != NULL )
        while( T->Right != NULL )
            T = T->Right;

    return T;
}

// Internal Find routine for SearchTree:
// Return pointer to node
// containing X or NULL if X is not found

template <class Etype>
const BinaryNode<Etype> *
SearchTree<Etype>::Find( const Etype & X, const BinaryNode<Etype> * T ) const
{
    while( T != NULL )
        if( X < T->Element )
            T = T->Left;
        else if( T->Element < X )
            T = T->Right;
        else
            return T;   // Match

    return NULL;        // Not found
}
