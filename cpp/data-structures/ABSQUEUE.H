#ifndef __AbsQueue
#define __AbsQueue

// Queue abstract class interface
//
// Etype: must have zero-parameter constructor;
//     implementation will require either
//     operator= or copy constructor, perhaps both
// CONSTRUCTION: with (a) no initializer;
//     copy construction of Queue objects is DISALLOWED
//
// ******************PUBLIC OPERATIONS*********************
//     All of the following are pure virtual functions
// void Enqueue( Etype X )--> Insert X
// void Dequeue( )        --> Remove least recently inserted item
// Etype Front( )         --> Return least recently inserted item
// int IsEmpty( )         --> Return 1 if empty; else return 0
// int IsFull( )          --> Return 1 if full; else return 0
// void MakeEmpty( )      --> Remove all items
// ******************ERRORS********************************
// Front or Dequeue on empty queue

template <class Etype>
class AbsQueue
{
  public:
    AbsQueue( ) { }               // Default constructor
    virtual ~AbsQueue( ) { }      // Destructor

    virtual void Enqueue( const Etype & X ) = 0;    // Insert
    virtual void Dequeue( ) = 0;                    // Remove
    virtual const Etype & GetFront( ) const = 0;    // Find
    virtual int IsEmpty( ) const = 0;
    virtual int IsFull( ) const = 0;
    virtual void MakeEmpty( ) = 0;
  private:
        // Disable copy constructor
    AbsQueue( const AbsQueue & ) { }
};
#endif
