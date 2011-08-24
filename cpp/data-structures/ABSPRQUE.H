#ifndef __AbsPrQue
#define __AbsPrQue

// Abstract class is not used because FindMin
// turns out to be a non-const member in the
// binary heap implementation, and also because
// construction there requires a sentinel.
// See Heap.h for a description of the class.

template <class Etype>
class AbsPrQueue
{
  public:
    AbsPrQueue( ) { }
    virtual ~AbsPrQueue( ) { }

        // Add an item maintaining heap order
    virtual void Insert( const Etype & X ) = 0;
        // Delete minimum item in heap
    virtual void DeleteMin( ) = 0;
    virtual void DeleteMin( Etype & X ) = 0;
        // Return minimum item in heap
    virtual const Etype & FindMin( ) const = 0;

    virtual int IsEmpty( ) const = 0;
    virtual int IsFull( ) const = 0;
    virtual void MakeEmpty( ) = 0;
  private:
    AbsPrQueue( const AbsPrQueue & );
};
#endif
