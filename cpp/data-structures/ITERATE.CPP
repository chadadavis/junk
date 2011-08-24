#include "Iterate.h"
#include <stdlib.h>

//////////// Base class

template <class Etype>
const Etype &
TreeIterator<Etype>::operator( ) ( ) const
{
    EXCEPTION( Current == NULL, "Illegal access!" );
    return Current->Element;
}


//////////// PREORDER

template <class Etype>
PreOrder<Etype>::PreOrder( const SearchTree<Etype> & TheTree ) :
        TreeIterator<Etype>( TheTree )
{
    S.Push( T.GetRoot( ) );
}

template <class Etype>
void
PreOrder<Etype>::First( )
{
    S.MakeEmpty( );
    if( T.GetRoot( ) )
        S.Push( T.GetRoot( ) );
    operator++( );
}

template <class Etype>
void
PreOrder<Etype>::operator++( )
{
    if( S.IsEmpty( ) )
    {
        EXCEPTION( Current == NULL, "Advanced past end" );
        Current = NULL;
        return;
    }

    Current = S.Top( );
    S.Pop( );
    if( Current->Right )
        S.Push( Current->Right );
    if( Current->Left )
        S.Push( Current->Left );
}

////////// POSTORDER

template <class Etype>
PostOrder<Etype>::PostOrder( const SearchTree<Etype> & TheTree ) :
                TreeIterator<Etype>( TheTree )
{
    S.Push( StNode<Etype>( T.GetRoot( ) ) );
}

template <class Etype>
void
PostOrder<Etype>::First( )
{
    S.MakeEmpty( );
    if( T.GetRoot( ) )
        S.Push( StNode<Etype>( T.GetRoot( ) ) );
    operator++( );
}

template <class Etype>
void
PostOrder<Etype>::operator++( )
{
    if( S.IsEmpty( ) )
    {
        EXCEPTION( Current == NULL, "Advanced past end" );
        Current = NULL;
        return;
    }

    StNode <Etype> Cnode;

    for( ; ; )
    {
        Cnode = S.Top( );
        S.Pop( );
    
        if( ++Cnode.TimesPopped == 3 )
        {
            Current = Cnode.Node;
            return;
        }
    
        S.Push( Cnode );
        if( Cnode.TimesPopped == 1 )
        {
            if( Cnode.Node->Left )
                S.Push( StNode<Etype>( Cnode.Node->Left ) );
        }
        else  // Cnode.TimesPopped == 2
        {
            if( Cnode.Node->Right )
                S.Push( StNode<Etype>( Cnode.Node->Right ) );
        }
    }
}


////////// INORDER

template <class Etype>
void
InOrder<Etype>::operator++( )
{

    if( S.IsEmpty( ) )
    {
        EXCEPTION( Current == NULL, "Advanced past end" );
        Current = NULL;
        return;
    }

    StNode<Etype> Cnode;

    for( ; ; )
    {
        Cnode = S.Top( );
        S.Pop( );
    
        if( ++Cnode.TimesPopped == 2 )
        {
            Current = Cnode.Node;
            if( Cnode.Node->Right )
                S.Push( StNode<Etype>( Cnode.Node->Right ) );
            return;
        }
    
            // First time through
        S.Push( Cnode );
        if( Cnode.Node->Left )
            S.Push( StNode<Etype>( Cnode.Node->Left ) );
    }
}


////////// LEVEL ORDER

template <class Etype>
LevelOrder<Etype>::LevelOrder( const SearchTree<Etype> & TheTree ) :
           TreeIterator<Etype>( TheTree )
{
    Q.Enqueue( T.GetRoot( ) );
}

template <class Etype>
void
LevelOrder<Etype>::First( )
{
    Q.MakeEmpty( );
    if( T.GetRoot( ) )
        Q.Enqueue( T.GetRoot( ) );
    operator++( );
}

template <class Etype>
void
LevelOrder<Etype>::operator++( )
{
    if( Q.IsEmpty( ) )
    {
        EXCEPTION( Current == NULL, "Advanced past end" );
        Current = NULL;
        return;
    }

    Current = Q.GetFront( );
    Q.Dequeue( );
    if( Current->Left )
        Q.Enqueue( Current->Left );
    if( Current->Right )
        Q.Enqueue( Current->Right );
}
