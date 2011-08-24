#ifndef __AbsStack
#define __AbsStack

// Stack abstract class interface
//
// Etype: must have zero-parameter constructor;
//     implementation will require either
//     operator= or copy constructor, perhaps both
// CONSTRUCTION: with (a) no initializer;
//     copy construction of Stack objects is DISALLOWED
//
// ******************PUBLIC OPERATIONS*********************
//     All of the following are pure virtual functions
// void Push( Etype X )   --> Insert X
// void Pop( )            --> Remove most recently inserted item
// Etype Top( )           --> Return most recently inserted item
// int IsEmpty( )         --> Return 1 if empty; else return 0
// int IsFull( )          --> Return 1 if full; else return 0
// void MakeEmpty( )      --> Remove all items
// ******************ERRORS********************************
// Top or Pop on empty stack

template <class Etype>
class AbsStack
{
  public:
    AbsStack( ) { }                 // Default constructor
    virtual ~AbsStack( ) { }        // Destructor

    virtual void Push( const Etype & X ) = 0;   // Insert
    virtual void Pop( ) = 0;                    // Remove
    virtual const Etype & Top( ) const = 0;     // Find
    virtual int IsEmpty( ) const = 0;
    virtual int IsFull( ) const = 0;
    virtual void MakeEmpty( ) = 0;
  private:
    AbsStack( const AbsStack & ) { }
};
#endif
