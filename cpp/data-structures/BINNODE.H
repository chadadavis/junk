#ifndef __BinNode
#define __BinNode
#include <stdlib.h>

template <class Etype>
struct BinaryNode
{
    Etype Element;       // The data in the node
    BinaryNode *Left;    // Left child
    BinaryNode *Right;   // Right child

    int Size;            // Node's size  (used for order stats)
    int BalancingInfo;   // Information used for balancing

    BinaryNode( ) : Left( NULL ), Right( NULL ),
        Size( 1 ), BalancingInfo( 1 ) { }
    BinaryNode( const Etype & E ) : Element( E ), Left( NULL ),
        Right( NULL ), Size( 1 ), BalancingInfo( 1 ) { }
    BinaryNode( const Etype & E, BinaryNode *L, BinaryNode * R ) :
        Element( E ), Left( L ), Right( R ), Size( 1 ),
        BalancingInfo( 1 ) { }

    ~BinaryNode( ) { }
};
#endif
