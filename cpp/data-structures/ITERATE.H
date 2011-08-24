#ifndef __Iterate
#define __Iterate
#include "Bst.h"
#include "Stack.h"
#include "Exception.h"

#ifdef __GNUC__
    #include "Stack.cpp"
#endif

//////////// ITERATOR BASE CLASS

// TreeIterator class interface; maintains "current position"
//
// Etype: same restrictions as for BinaryTree
// CONSTRUCTION: with (a) Tree to which iterator is bound
// All copying of TreeIterator objects is DISALLOWED
//
// ******************PUBLIC OPERATIONS**********************
//     First two are not virtual, last two are pure virtual
// int operator+( )     --> True if at valid position in tree
// Etype operator( )    --> Return item in current position
// void First( )        --> Set current position to first
// void operator++      --> Advance (prefix)
// ******************ERRORS*********************************
// EXCEPTION is called for illegal access or advance

template <class Etype>
class TreeIterator
{
  public:
    TreeIterator( const SearchTree<Etype> & TheTree ) :
        T( TheTree ), Current( NULL ) { }
    virtual ~TreeIterator( ) { }

    virtual void First( ) = 0;
    int operator+ ( ) const { return Current != NULL; }
    const Etype & operator( ) ( ) const;
    virtual void operator++(  ) = 0;
  protected:
    const SearchTree<Etype> & T;
    const BinaryNode<Etype> *Current;

        // Disable copy constructor and assignment
    TreeIterator( const TreeIterator & );
    const TreeIterator & operator= ( const TreeIterator & );
};


//////////// PREORDER

// PreOrder class interface; maintains "current position"
//
// Etype: same restrictions as for BinaryTree
// CONSTRUCTION: with (a) Tree to which iterator is bound
// All copying of PreOrder objects is DISALLOWED
//
// ******************PUBLIC OPERATIONS**********************
// int operator+( )     --> True if at valid position in tree
// Etype operator( )    --> Return item in current position
// void First( )        --> Set current position to first
// void operator++      --> Advance (prefix)
// ******************ERRORS*********************************
// EXCEPTION is called for illegal access or advance

template <class Etype>
class PreOrder: public TreeIterator<Etype>
{
  public:
    PreOrder( const SearchTree<Etype> & TheTree );
    ~PreOrder( ) { }

    void First( );
    void operator++( );
  protected:
    Stack< const BinaryNode<Etype> * > S;
};

////////// POSTORDER

// PostOrder class interface; maintains "current position"
//
// Etype: same restrictions as for BinaryTree
// CONSTRUCTION: with (a) Tree to which iterator is bound
// All copying of PostOrder objects is DISALLOWED
//
// ******************PUBLIC OPERATIONS**********************
// int operator+( )     --> True if at valid position in tree
// Etype operator( )    --> Return item in current position
// void First( )        --> Set current position to first
// void operator++      --> Advance (prefix)
// ******************ERRORS*********************************
// EXCEPTION is called for illegal access or advance

template <class Etype>
struct StNode
{
    const BinaryNode<Etype> *Node;
    int TimesPopped;

    StNode( const BinaryNode<Etype> *N = 0 )
            : Node( N ), TimesPopped( 0 ) { }
};

template <class Etype>
class PostOrder : public TreeIterator<Etype>
{
  public:
    PostOrder( const SearchTree<Etype> & TheTree );
    ~PostOrder( ) { }

    void First( );
    void operator++( );
  protected:
    Stack< StNode<Etype> > S;
};


////////// INORDER

// InOrder class interface; maintains "current position"
//
// Etype: same restrictions as for BinaryTree
// CONSTRUCTION: with (a) Tree to which iterator is bound
// All copying of InOrder objects is DISALLOWED
//
// ******************PUBLIC OPERATIONS**********************
// int operator+( )     --> True if at valid position in tree
// Etype operator( )    --> Return item in current position
// void First( )        --> Set current position to first
// void operator++      --> Advance (prefix)
// ******************ERRORS*********************************
// EXCEPTION is called for illegal access or advance

template <class Etype>
class InOrder : public PostOrder<Etype>
{
    // Accept PostOrder construction and default destruction.

  public:
    InOrder( const SearchTree<Etype> & TheTree ) :
        PostOrder<Etype>( TheTree ) { }
    void operator++( );
};

////////// LEVEL ORDER

// LevelOrder class interface; maintains "current position"
//
// Etype: same restrictions as for BinaryTree
// CONSTRUCTION: with (a) Tree to which iterator is bound
// All copying of LevelOrder objects is DISALLOWED
//
// ******************PUBLIC OPERATIONS**********************
// int operator+( )     --> True if at valid position in tree
// Etype operator( )    --> Return item in current position
// void First( )        --> Set current position to first
// void operator++      --> Advance (prefix)
// ******************ERRORS*********************************
// EXCEPTION is called for illegal access or advance

template <class Etype>
class LevelOrder : public TreeIterator<Etype>
{
  public:
    LevelOrder( const SearchTree<Etype> & TheTree );
    ~LevelOrder( ) { }

    void First( );
    void operator++( );
  private:
    Queue< const BinaryNode<Etype> * > Q;
};
#endif
