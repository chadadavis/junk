// Queue class interface: array implementation
//
// Etype: must have zero-parameter constructor and operator=
// CONSTRUCTION: with (a) no initializer;
//     copy construction of Queue objects is DISALLOWED
// Deep copy is supported
//
// ******************PUBLIC OPERATIONS*********************
// void Enqueue( Etype X )--> Insert X
// void Dequeue( )        --> Remove least recently inserted item
// Etype Front( )         --> Return least recently inserted item
// int IsEmpty( )         --> Return 1 if empty; else return 0
// int IsFull( )          --> Return 1 if full; else return 0
// void MakeEmpty( )      --> Remove all items
// ******************ERRORS********************************
// Predefined exception is propogated if new fails
// EXCEPTION is called for Front or Dequeue on empty queue

#ifndef __QueueAr
#define __QueueAr
#include "AbsQueue.h"

// Array-based queue
template <class Etype>
class Queue : public AbsQueue<Etype>
{
  public:
    Queue( );
    ~Queue( ) { delete [ ] Array; }

    const Queue & operator=( const Queue & Rhs );

    void Enqueue( const Etype & X );
    void Dequeue( );
    const Etype & GetFront( ) const;
    int IsEmpty( ) const   { return CurrentSize == 0; }
    int IsFull( ) const    { return 0; }
    void MakeEmpty( );
  private:
    // Copy constructor remains disabled by inheritance

    int MaxSize;
    int CurrentSize;
    int Front;
    int Back;
    Etype *Array;

    void DoubleQueue( );             // Double array -- complicated
    void Increment( int & X ) const; // Add one with wraparound
};
#endif
