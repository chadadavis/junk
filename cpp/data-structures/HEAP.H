// BinaryHeap class interface
//
// Etype: must have zero-parameter constructor and operator=;
//     must have operator<
// CONSTRUCTION: with (a) Etype representing negative infinity
// Copy construction of BinaryHeap objects is DISALLOWED
// Deep copy is supported
//
// ******************PUBLIC OPERATIONS******************
// void Inesrt( Etype X ) --> Insert X
// Etype FindMin( )       --> Return smallest item
// void DeleteMin( )      --> Remove smallest item
// void DeleteMin( Etype & X ) --> Same, but put it in X
// int IsEmpty( )         --> Return 1 if empty; else return 0
// int IsFull( )          --> Return 1 if full; else return 0
// void MakeEmpty( )      --> Remove all items
// void Toss( Etype X )   --> Insert X (lazily)
// void FixHeap( )        --> Reestablish heap order property
// ******************ERRORS*****************************
// Predefined exception is propogated if new fails
// EXCEPTION is called for FindMin or DeleteMin when empty

#ifndef __BinaryHeap
#define __BinaryHeap

template <class Etype>
class BinaryHeap
{
  public:
        // Constructor, destructor, and copy assignment
    BinaryHeap( const Etype & MinVal );
    ~BinaryHeap( ) { delete [ ] Array; }

    const BinaryHeap & operator=( const BinaryHeap & Rhs );

        // Add an item maintaining heap order
    void Insert( const Etype & X );

        // Add an item but do not maintain order
    void Toss( const Etype & X );

        // Return minimum item in heap
    const Etype & FindMin( );

        // Delete minimum item in heap
    void DeleteMin( );
    void DeleteMin( Etype & X );

        // Reestablish heap order (linear-time function)
    void FixHeap( );

    int IsEmpty( ) const { return CurrentSize == 0; }
    int IsFull( ) const  { return 0; }
    void MakeEmpty( )    { CurrentSize = 0; }
  private:
    enum { DefaultSize = 10 };

    int MaxSize;         // Number of elements that can be stored
    int CurrentSize;     // Number of elements currently stored
    int OrderOK;         // Zero if heap order is not guaranteed
    Etype *Array;        // Dynamically allocated array

    BinaryHeap( const BinaryHeap & ); // Disable copy constructor

        // Internal routines
    void PercolateDown( int Index );
    void GetArray( int NewMaxSize );  // Allocate array
    void CheckSize( );   // Used for Toss and Insert
};
#endif
