#include "List.h"
#include "Exception.h"

template <class Etype>
const List<Etype> &
List<Etype>::operator=( const List<Etype> & Rhs )
{
    if( this == &Rhs )
        return *this;

    MakeEmpty( );

    ListItr<Etype> Itr( *this );
    
    for( ListItr<Etype> Ritr( Rhs ); +Ritr; Ritr++ )
        Itr.Insert( Ritr( ) );

    return *this;
}


template <class Etype>
void
List<Etype>::MakeEmpty( )
{
    ListNode *Ptr;
    ListNode *NextNode;

    for( Ptr = Header->Next; Ptr != NULL; Ptr = NextNode )
    {
        NextNode = Ptr->Next;
        delete Ptr;
    }
    Header->Next = NULL;
}

template <class Etype>
ostream &
operator<<( ostream & Out, const List<Etype> & L )
{
    if( L.IsEmpty( ) )
        Out << "Empty list";
    else
        for( ListItr<Etype> Itr( L ); +Itr; ++Itr )
            Out << Itr( ) << ' ';

    return Out << '\n';
}


template <class Etype>
void
ListItr<Etype>::Insert( const Etype & X )
{
    EXCEPTION( Current == NULL, "Illegal insertion" );
    List<Etype>::ListNode *NewNode;

    NewNode = new List<Etype>::ListNode( X, Current->Next );
    Current = Current->Next = NewNode;
}

template <class Etype>
int
ListItr<Etype>::Find( const Etype & X )
{
    List<Etype>::ListNode *Ptr = Header->Next;

    while( Ptr != NULL && Ptr->Element != X )
        Ptr = Ptr->Next;

    if( Ptr == NULL )
        return 0;

    Current = Ptr;
    return 1;
}

template <class Etype>
int
ListItr<Etype>::Remove( const Etype & X )
{
    List<Etype>::ListNode *Ptr = Header;

    while( Ptr->Next != NULL && Ptr->Next->Element != X )
        Ptr = Ptr->Next;

    if( Ptr->Next == NULL )
        return 0;    // Remove fails

        // Bypass and reclaim memory
    List<Etype>::ListNode *DeletedNode = Ptr->Next;
    Ptr->Next = Ptr->Next->Next;
    delete DeletedNode;

    Current = Header;    // Reset Current
    return 1;
}

template <class Etype>
int
ListItr<Etype>::RemoveNext( )
{
    if( Current == NULL || Current->Next == NULL )
        return 0;

    List <Etype>::ListNode *DeletedNode = Current->Next;
    Current->Next = Current->Next->Next;
    delete DeletedNode;

    return 1;
}

template <class Etype>
int
ListItr<Etype>::IsFound( const Etype & X ) const
{
    List<Etype>::ListNode *Ptr = Header->Next;

    while( Ptr != NULL && Ptr->Element != X )
        Ptr = Ptr->Next;

    return Ptr != NULL;
}

template <class Etype>
const Etype &
ListItr<Etype>::operator( ) ( ) const
{
    EXCEPTION( Current == Header || Current == NULL, "Illegal access" );

    return Current->Element;
}

template <class Etype>
void
ListItr<Etype>::First( )
{
    EXCEPTION( Header->Next == NULL, "Illegal access - empty list"  );
    Current = Header->Next;
}

template <class Etype>
void
ListItr<Etype>::operator++( )
{
    EXCEPTION( Current == NULL, "Can't advance any further" );
    Current = Current->Next;
}

template <class Etype>
void
SortListItr<Etype>::Insert( const Etype & X )
{
    ListItr<Etype> Prev = TheList;
    ListItr<Etype> Curr = TheList;

    for( Prev.Zeroth( ); +Curr && Curr( ) < X; ++Prev, ++Curr )
        ;

    Prev.Insert( X );
    Current = Prev.Current;
}
