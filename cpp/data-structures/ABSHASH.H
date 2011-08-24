#ifndef __AbsHash
#define __AbsHash

// HashTable abstract class interface
//
// Etype: must have zero-parameter constructor and
//     operator==; implementation may require operator!=;
//     implementation will require either
//     operator= or copy constructor, perhaps both
// CONSTRUCTION: with (a) no initializer;
//     copy constructor of HahsTable objects is DISALLOWED
//
// ******************PUBLIC OPERATIONS*********************
//     All of the following are pure virtual functions
// int Insert( Etype X )  --> Insert X
// int Remove( Etype X )  --> Remove X
// Etype Find( Etype X )  --> Return item that matches X
// int WasFound( )        --> Return 1 if last Find succeeded
// int IsFound( Etype X ) --> Return 1 if X would be found
// int IsEmpty( )         --> Return 1 if empty; else return 0
// int IsFull( )          --> Return 1 if full; else return 0
// void MakeEmpty( )      --> Remove all items

template <class Etype>
class AbsHTable
{
  public:
    AbsHTable( ) { }
    virtual ~AbsHTable( ) { }

    virtual int Insert( const Etype & X ) = 0;
    virtual int Remove( const Etype & X ) = 0;
    virtual const Etype & Find( const Etype & X ) = 0;
    virtual int WasFound( ) const = 0;
    virtual int IsFound( const Etype & X ) const = 0;
    virtual int IsEmpty( ) const = 0;
    virtual int IsFull( ) const = 0;
    virtual void MakeEmpty( ) = 0;

  private:
        // Disable copy constructor
    AbsHTable( const AbsHTable & ) { }
};
#endif
