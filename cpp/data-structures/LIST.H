// List class interface
//
// Etype: must have zero-parameter and copy constructor
//     operator!= must be provided
// CONSTRUCTION: with (a) no initializer;
//     copy construction of List objects is DISALLOWED
// Deep copy is supported
// Access is via ListItr class
//
// ******************PUBLIC OPERATIONS*********************
// int IsEmpty( )         --> Return 1 if empty; else return 0
// int IsFull( )          --> Return 1 if full; else return 0
// void MakeEmpty( )      --> Remove all items
// ******************ERRORS********************************
// Predefined exception is propogated if new fails

#ifndef __List__
#define __List__
#include <iostream.h>
#include <stdlib.h>
#include "AbsList.h"

template <class Etype>   // Incomplete class declaration
class ListItr;           // so friend is visible

template <class Etype>
class List : public AbsList<Etype>
{
  public:
    List( )  { Header = new ListNode; }
    ~List( ) { MakeEmpty( ); delete Header; }

    const List & operator=( const List & Rhs );

    int IsEmpty( ) const { return Header->Next == NULL; }
    int IsFull( )  const { return 0; }
    void MakeEmpty( );

    friend class ListItr<Etype>;
  private:
    struct ListNode
    {
        Etype Element;
        ListNode *Next;

        ListNode( ) : Next( NULL ) { }
        ListNode( const Etype & E, ListNode * N = NULL ) :
                Element( E ), Next( N ) { }
    };

    ListNode *Header;     // Pointer to a header node
};

template <class Etype>
ostream & operator<<( ostream & Out, const List<Etype> & L );


// ListItr class interface; maintains "current position"
//
// Etype: same restrictions as for List
// CONSTRUCTION: with (a) List to which ListItr is permanently
//     bound or (b) another ListItr;
//     Copying of ListItr objects not supported in current form
//
// ******************PUBLIC OPERATIONS*********************
// void Insert( Etype X ) --> Insert X after current position
// int Remove( Etype X )  --> Remove X
// int Find( Etype X )    --> Set current position to view X
// int IsFound( Etype X ) --> Return 1 if X would be found
// void Zeroth( )         --> Set current position to prior to first
// void operator++        --> Advance (both prefix and postfix)
// int operator+( )       --> True if at valid position in list
// Etype operator( )      --> Return item in current position
// ******************ERRORS********************************
// Predefined exception is propogated if new fails
// EXCEPTION is called for illegal access, advance,
//     insertion, or First on empty list

template <class Etype>
class ListItr : public AbsListItr<Etype>
{
  public:
    ListItr( const List<Etype> & L ) : Header( L.Header )
            { Current = L.IsEmpty( ) ? Header : Header->Next; }
    ~ListItr( ) { }

    // Insert X after Current position
    // Errors: Current position is NULL or memory is exhausted
    // Current: Set to new node on success; unchanged otherwise
    void Insert( const Etype & X );

    // Set Current position to first node containing X
    // Returns: 1 if Find is successful, 0 for failure
    // Current: is unchanged if X is found
    int Find( const Etype & X );

    // Returns 1 if X is in the list, 0 otherwise
    int IsFound( const Etype & X ) const;

    // Removes first occurrence of X; do nothing if X is not found
    int Remove( const Etype & X );

    // Remove next cell
    int RemoveNext( );

    // Returns 1 if Current is not NULL or Header, 0 otherwise
    int operator+( ) const { return Current && Current != Header; }

    // Returns the Element in Current node
    // Errors: If Current is pointing at a node in the list
    const Etype & operator( ) ( ) const;

    // Set Current to the header node
    void Zeroth( )      { Current = Header; }

    // Set Current to first node in the list
    // Errors: If List is empty
    void First( );

    // Set Current to Current->Next; no return value
    // Errors: If Current is NULL on entry
    void operator++( );
    void operator++( int ) { operator++( ); }
  private:
    List<Etype>::ListNode * const Header;   // List Header
    List<Etype>::ListNode *Current;         // Current position
    friend class SortListItr<Etype>;
};

// SortListItr class interface; maintains "current position"
//
// Etype: same restrictions as for List plus operator< is needed
// CONSTRUCTION: with (a) List to which ListItr is permanently
//     bound or (b) another ListItr;
//     Copying of ListItr objects not supported in current form
//
// ******************PUBLIC OPERATIONS*********************
// void Insert( Etype X ) --> Insert X in sorted order
// int Remove( Etype X )  --> Remove X
// int Find( Etype X )    --> Set current position to view X
// int IsFound( Etype X ) --> Return 1 if X would be found
// void Zeroth( )         --> Set current position to prior to first
// void operator++        --> Advance (both prefix and postfix)
// int operator+( )       --> True if at valid position in list
// Etype operator( )      --> Return item in current position
// ******************ERRORS********************************
// Predefined exception is propogated if new fails
// EXCEPTION is called for illegal access, advance,
//     insertion, or First on empty list

template <class Etype>
class SortListItr : public ListItr<Etype>
{
  public:
    SortListItr( const List<Etype> & L ) : ListItr<Etype>( L ),
            TheList( L ) { }
    ~SortListItr( ) { }

        // Everything is same except Insert
    void Insert( const Etype & X );   // Insert in sorted order
  private:
    const List<Etype> & TheList;
};
#endif
