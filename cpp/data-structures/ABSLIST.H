#ifndef __AbsList
#define __AbsList


// List abstract class interface
//
// Etype: must have zero-parameter constructor;
//     implementation will require either
//     operator= or copy constructor, perhaps both;
//     implementation will require either
//     operator== or operator!=, perhaps both
// CONSTRUCTION: with (a) no initializer;
//     copy construction of List objects is DISALLOWED
//
// ******************PUBLIC OPERATIONS*********************
//     All of the following are pure virtual functions
// int IsEmpty( )         --> Return 1 if empty; else return 0
// int IsFull( )          --> Return 1 if full; else return 0
// void MakeEmpty( )      --> Remove all items


// Incomplete class declaration
template <class Etype>
class AbsListItr;

template <class Etype>
class AbsList
{
  public:
    AbsList( ) { }
    virtual ~AbsList( ) { }

    // Basic Members
    virtual int IsEmpty( ) const = 0;
    virtual int IsFull( )  const = 0;
    virtual void MakeEmpty( ) = 0;

    friend class AbsListItr<Etype>;
  private:
    AbsList( const AbsList & ) { } // Disable copy constructor
};


// ListItr abstract class interface
//
// Etype: same restrictions as for List
// CONSTRUCTION: with (a) List to which ListItr is permanently
//     bound or (b) another ListItr;
//     copying of ListItr objects is allowed only if both the
//     source and target are bound to the same list
//
// ******************PUBLIC OPERATIONS*********************
//     All of the following are pure virtual functions
// int Insert( Etype X )  --> Insert X after current position
// int Remove( Etype X )  --> Remove X
// int Find( Etype X )    --> Set current position to view X
// int IsFound( Etype X ) --> Return 1 if X would be found
// void Zeroth( )         --> Set current position to prior to first
// void operator++        --> Advance (both prefix and postfix)
// int operator+( )       --> True if at valid position in list
// Etype operator( )      --> Return item in current position

template <class Etype>
class AbsListItr
{
  public:
    AbsListItr( const AbsList<Etype> & L ) { }  // Constructor
    AbsListItr( const AbsListItr & );      // Copy constructor
    virtual ~AbsListItr( ) { }                   // Destructor

        // Note: implementation is given here; in text it is not
    virtual const AbsListItr & operator=( const AbsListItr & )
        { return *this; }

    // Basic Members: These return success or fail
    virtual void Insert( const Etype & X ) = 0;  // after current
    virtual int Find( const Etype & X ) = 0;     // goto X
    virtual int Remove( const Etype & X ) = 0;   // Remove X
    virtual int IsFound( const Etype & X ) const = 0;

    // Access operators
    virtual int operator+( ) const = 0;          // True if in list
    virtual const Etype & operator( ) ( ) const = 0;

    // Iteration operators
    virtual void Zeroth( ) = 0;                  // Go prior to first item
    virtual void First( ) = 0;                   // Go to first item
    virtual void operator++( ) = 0;              // Advance
    virtual void operator++( int ) = 0;
  protected:
    // Real ListItr will have a reference to the list as data member

    AbsListItr( ) { } // Disallow default construction
};
#endif
