#include <strstream.h>
#include <fstream.h>
#include <stdlib.h>
#include <iostream.h>
#include <limits.h>      // Used for infinity (for integers)

#include "String.h"
#include "Queue.h"
#include "List.h"
#include "Hash.h"
#include "Heap.h"
#include "Vector.h"

#ifdef __GNUC__
    #include "String.cpp"
    #include "Queue.cpp"
    #include "List.cpp"
    #include "Hash.cpp"
    #include "Heap.cpp"
    #include "Vector.cpp"
#endif

#define DijkstraBinaryHeap  // Use binary heaps for Dijkstra's algorithm

// Basic item in the adjacency list

template <class DistType>
struct Edge
{
                          // First vertex in edge is implicit
    int Dest;             // Second vertex in edge
    DistType Cost;        // Edge cost

    Edge( ) { }
    Edge( int D, DistType C ) : Dest( D ), Cost( C ) { }

    int operator!=( const Edge & ) const  // Not used
        { return 1; }
};

// Basic item stored for each vertex

template <class NameType, class DistType>
struct Vertex
{
    NameType Name;        // Real name
    List< Edge<DistType> > *Adj;      // Adjacent vertices

    DistType Dist;        // Cost (after running algorithm)
    int Prev;             // Previous vertex on shortest path
    int Scratch;          // Extra variable for use in algorithm
};

// The basic entry in the vertex dictionary

template <class NameType>
struct HashItem
{
    NameType Name;
    int Rank;

    HashItem( ) { }
    HashItem( const NameType & Nm ) : Name( Nm ) { }

    int operator==( const HashItem & Rhs ) const
        { return Name == Rhs.Name; }
    int operator!=( const HashItem & Rhs ) const
        { return Name != Rhs.Name; }
};

// Hash function for HashItem
// Calls hash function for NameType

template <class NameType>
unsigned int
Hash( const HashItem<NameType> & Key, int TableSize )
{
    return Hash( Key.Name, TableSize );
}

// Standard String hash function
    
unsigned int
Hash( const String & Key, int TableSize )
{
    unsigned int HashVal = 0;
    int i = 0;

    while( Key[ i ] != '\0' )
        HashVal = ( HashVal << 5 ) ^ HashVal ^ Key[ i++ ];

    return HashVal % TableSize;
}

// Graph class interface: evaluate shortest paths
// NameType: Must have a hash function, copy constructor,
//     operator=, operator>>, operator<<, zero-parameter constructor
// DistType: Must have conversion from infinity, copy contructor,
//     operator=, operator>>, operator<<, zero-parameter constructor,
//     int contructor, +, <, >, ==
//
// CONSTRUCTION: with (a) Infinity and optional const char *
//     that stores edges in input graph;
//     copy constructor is disabled
//
// ******************PUBLIC OPERATIONS**********************
// void AddEdges( NameType V, NameType W, DistType Cvw )
//                      --> Add additional edge
// int ProcessRequest( )--> Run a bunch of shortest path algs
// ******************ERRORS*********************************
// Some error checking is performed to make sure graph is ok,
// parameters to ProcessRequest represent vertices in the graph,
// and to make sure graph satisfies properties needed by each algorithm

template<class NameType, class DistType>
class Graph
{
  public:
    Graph( DistType Inf, const char *FileName = NULL );
    ~Graph( );

    void AddEdge( const NameType & Source, const NameType & Dest,
                  DistType Cost );
    int ProcessRequest( );
  private:
        // Disable copy constructor and copy assignment
    Graph( const Graph & );
    const Graph & operator= ( const Graph & );

    enum { InitTableSize = 50, NullVertex = -1 };

    const DistType Infinity;                   // For initialization
    HashTable< HashItem<NameType> > VertexMap; // Gets internal #
    Vector< Vertex<NameType,DistType> > Table; // The table array
    int NumVertices;               // Current # vertices read

        // If VertexName is an already seen vertex, return its
        // internal number. Otherwise, add it as a new vertex,
        // and return its ew internal number
    int AddNode( const NameType & VertexName );

        // Add an edge given internal numbers of its vertices
    void AddInternalEdge( int Source, int Dest, DistType Const );

    void ClearData( );      // Initialize the table

        // Read graph from stream requested in constructor
    void ReadGraph( ifstream & GraphStream );

        // Print the shortest path to DestNode
        // (specified by its internal number)
        // PrintPath is the driver routine
    void PrintPathRec( int DestNode ) const;
    void PrintPath( int DestNode ) const;

        // Various shortest path algorithms that require
        // an internal number for start-up
    void Unweighted( int StartNode );
    int Dijkstra( int StartNode );
    int Negative( int StartNode );
    int Acyclic( int StartNode );
};

template <class NameType, class DistType>
Graph<NameType, DistType>::~Graph( )
{
    for( int i = 0; i < NumVertices; i++ )
        delete Table[ i ].Adj;
}

// Prompt user for two vertices and run all shortest path
// algorithms between the vertices

template <class NameType, class DistType>
int
Graph<NameType, DistType>::ProcessRequest( )
{
    NameType SourceName, DestName;
    static HashItem<NameType> Source, Dest;

    do
    {
        cout << "Enter start node: ";
        if( !( cin >> SourceName ) )
            return 0;
        Source.Name = SourceName;
        Source = VertexMap.Find( Source );
    } while( !VertexMap.WasFound( ) );
    
    do
    {
        cout << "Enter destination node: ";
        if( !( cin >> DestName ) )
            return 0;;
        Dest.Name = DestName;
        Dest = VertexMap.Find( Dest );
    } while( !VertexMap.WasFound( ) );
    
    Unweighted( Source.Rank );
    PrintPath( Dest.Rank );
    if( Dijkstra( Source.Rank ) )
        PrintPath( Dest.Rank );
    if( Negative( Source.Rank ) )
        PrintPath( Dest.Rank );
    if( Acyclic( Source.Rank ) )
        PrintPath( Dest.Rank );

    return 1;
}

// Initialize the graph table prior to running
// any shortest path algorithm

template <class NameType, class DistType>
void
Graph<NameType, DistType>::ClearData( )
{
    for( int i = 0; i < NumVertices; i++ )
    {
        Table[ i ].Dist = Infinity;
        Table[ i ].Prev = -1;
        Table[ i ].Scratch = 0;
    }
}

// Single source unweighted shortest path algorithm

template <class NameType, class DistType>
void
Graph<NameType, DistType>::Unweighted( int StartNode )
{
    int V, W;
    Queue<int> Q;

    ClearData( );
    Table[ StartNode ].Dist = 0;
    Q.Enqueue( StartNode );

    while( !Q.IsEmpty( ) )
    {
        V = Q.GetFront( );
        Q.Dequeue( );
        for( ListItr< Edge<DistType> > P = *Table[ V ].Adj; +P; ++P )
        {
            W = P( ).Dest;
            if( Table[ W ].Dist == Infinity )
            {
                Table[ W ].Dist = Table[ V ].Dist + 1;
                Table[ W ].Prev = V;
                Q.Enqueue( W );
            }
        }
    }
}

// Single source acyclic weighted shortest path algorithm

template <class NameType, class DistType>
int
Graph<NameType, DistType>::Acyclic( int StartNode )
{
    int V, W, Iterations;
    Queue<int> Q;

    ClearData( );
    Table[ StartNode ].Dist = 0;

        // Compute indegrees
    for( V = 0; V < NumVertices; V++ )

        for( ListItr< Edge<DistType> > P = *Table[ V ].Adj; +P; ++P )
            Table[ P( ).Dest ].Scratch++;

        // Enqueue vertices of indegree zero
    for( V = 0; V < NumVertices; V++ )
        if( Table[ V ].Scratch == 0 )
            Q.Enqueue( V );

    for( Iterations = 0; !Q.IsEmpty( ); Iterations++ )
    {
        V = Q.GetFront( );
        Q.Dequeue( );
    
        for( ListItr<Edge<DistType> > P = *Table[ V ].Adj; +P; ++P )
        {
            W = P( ).Dest;
            if( --Table[ W ].Scratch == 0 )
                Q.Enqueue( W );

            if( Table[ V ].Dist == Infinity )
                continue;

            DistType Cvw = P( ).Cost;
            if( Table[ W ].Dist > Table[ V ].Dist + Cvw )

            {
                Table[ W ].Dist = Table[ V ].Dist + Cvw;
                Table[ W ].Prev = V;
            }
        }
    }

    if( Iterations == NumVertices )
        return 1;

    cerr << "Graph has a cycle!" << endl;
    return 0;
}

// Single source negative weighted shortest path algorithm

template <class NameType, class DistType>
int 
Graph<NameType, DistType>::Negative( int StartNode )
{
    int V, W;
    Queue<int> Q;
    DistType Cvw;

    ClearData( );
    Table[ StartNode ].Dist = 0;
    Q.Enqueue( StartNode );
    Table[ StartNode ].Scratch++;

    while( !Q.IsEmpty( ) )
    {
        V = Q.GetFront( );
        Q.Dequeue( );
        if( Table[ V ].Scratch++ > 2 * NumVertices )
        {
            cerr << "Negative cycle detected!" << endl;
            return 0;
        }

        for( ListItr< Edge<DistType> > P = *Table[ V ].Adj; +P; ++P )
        {
            W = P( ).Dest;
            Cvw = P( ).Cost;
            if( Table[ W ].Dist > Table[ V ].Dist + Cvw )
            {
                Table[ W ].Dist = Table[ V ].Dist + Cvw;
                Table[ W ].Prev = V;

                // Enqueue only if not already on the queue
                if( Table[ W ].Scratch++ % 2 == 0 )
                    Q.Enqueue( W );
                else
                    Table[ W ].Scratch++;   // In effect, adds 2
            }
        }
    }
    return 1;
}

// Structure to store in priority queue for Dijkstra's algorithm

template <class DistType>
struct Path
{
    int Dest;        // W
    DistType Cost;   // D(W)

    Path( int D = 0, DistType C = 0 ) : Dest( D ), Cost( C ) { }

    int operator<( const Path & Rhs ) const
        { return Cost < Rhs.Cost; }
};


//Single source unweighted shortest path algorithm

    // Both a binary heap and pairing heap implementation
    // Are provided; choose your poison.

#ifdef DijkstraBinaryHeap

template <class NameType, class DistType>
int 
Graph<NameType, DistType>::Dijkstra( int StartNode )
{
    int V, W;
    BinaryHeap< Path<DistType> > PQ( Path<DistType>( 0, 0 ) );
    Path<DistType> VRec;    // Stores the result of a DeleteMin

    ClearData( );
    Table[ StartNode ].Dist = 0;
    PQ.Insert( Path<DistType>( StartNode, 0 ) );

    for( int NodesSeen = 0; NodesSeen < NumVertices; NodesSeen++  )
    {
        do
        {
            if( PQ.IsEmpty( ) )
                return 1;
            PQ.DeleteMin( VRec );
        } while( Table[ VRec.Dest ].Scratch );

        V = VRec.Dest;
        Table[ V ].Scratch = 1;     // Mark vertex as being seen
        for( ListItr< Edge<DistType> > P = *Table[ V ].Adj; +P; ++P )
        {
            W = P( ).Dest;
            DistType Cvw = P( ).Cost;
            if( Cvw < 0 )
            {
                cerr << "Graph has negative edges" << endl;
                return 0;
            }
            if( Table[ W ].Dist > Table[ V ].Dist + Cvw )
            {
                Table[ W ].Dist = Table[ V ].Dist + Cvw;
                Table[ W ].Prev = V;
                PQ.Insert( Path<DistType>( W, Table[ W ].Dist ) );
            }
        }
    }
    return 1;
}

#else

#include "Pair.h"

#ifdef __GNUC__
    #include "Pair.cpp"
#endif

template <class NameType, class DistType>
int 
Graph<NameType, DistType>::Dijkstra( int StartNode )
{
    int V, W;
    PairHeap< Path<DistType> > PQ;
    Path<DistType> VRec;
    Vector<PairNode< Path<DistType> > * > HeapPositions( NumVertices );

    ClearData( );
    for( int i = 0; i < NumVertices; i++ )
        HeapPositions[ i ] = NULL;

    Table[ StartNode ].Dist = 0;
    HeapPositions[ StartNode ] =
        PQ.Insert( Path<DistType>( StartNode, 0 ) );


    while( !PQ.IsEmpty( ) )
    {
        PQ.DeleteMin( VRec );

        V = VRec.Dest;
        Table[ V ].Scratch = 1;
        for( ListItr< Edge<DistType> > P = *Table[ V ].Adj; +P; ++P )
        {
            W = P( ).Dest;
            DistType Cvw = P( ).Cost;
            if( Cvw < 0 )
            {
                cerr << "Graph has negative edges" << endl;
                return 0;
            }
            if( Table[ W ].Dist > Table[ V ].Dist + Cvw )
            {
                Table[ W ].Dist = Table[ V ].Dist + Cvw;
                Table[ W ].Prev = V;

                Path<DistType> NewVal( W, Table[ W ].Dist );
                if( HeapPositions[ W ] == NULL )
                    HeapPositions[ W ] = PQ.Insert( NewVal );
                else
                    PQ.DecreaseKey( HeapPositions[ W ], NewVal );
            }
        }
    }

    return 1;
}
    
#endif
    

// Recursive routine to print shortest path to DestNode
// after running shortest path algorithm

template <class NameType, class DistType>
void
Graph<NameType, DistType>::PrintPathRec( int DestNode ) const
{
    if( Table[ DestNode ].Prev != NullVertex )
    {
        PrintPathRec( Table[ DestNode ].Prev );
        cout << " to ";
    }
    cout << Table[ DestNode ].Name;
}

// Driver routine to handle unreachables and print total cost
// It calls recursive routine to print shortest path to
// DestNode after a hortest path algorithm has run

template <class NameType, class DistType>
void
Graph<NameType, DistType>::PrintPath( int DestNode ) const
{
    if( Table[ DestNode ].Dist == Infinity )
        cout << Table[ DestNode ].Name << " is unreachable";
    else
    {
        PrintPathRec( DestNode );
        cout << " (cost is " << Table[ DestNode ].Dist << ")";
    }
    cout << endl;
}

// Add the edge ( Source, Dest, Cost ) to the graph
// Source and Dest are NameType objects

template <class NameType, class DistType>
void
Graph<NameType,DistType>::AddEdge( const NameType & Source,
            const NameType & Dest, DistType Cost )
{
    AddInternalEdge( AddNode( Source ), AddNode( Dest ), Cost );
}


// Read edges from GraphStream
// Checks that each line has at least three components

template <class NameType, class DistType>
void
Graph<NameType,DistType>::ReadGraph( ifstream & GraphStream )
{
    const int MaxLineLength = 256;
    static char OneLine[ MaxLineLength + 1 ];
    NameType Source, Dest;
    DistType Cost;

    while( GraphStream.getline( OneLine, MaxLineLength ) )
    {
        istrstream LineStream( OneLine, MaxLineLength );
        if( LineStream >> Source &&
            LineStream >> Dest &&
            LineStream >> Cost )
            AddEdge( Source, Dest, Cost );
        else
            cerr << "Bad line: " << OneLine << endl;
    }

    cout << "Read " << NumVertices << "vertices" << endl;
}

        
template <class NameType, class DistType>    
Graph<NameType,DistType>::Graph( DistType Inf,
    const char *FileName ) : NumVertices( 0 ), Infinity( Inf ),
                             Table( InitTableSize )
{
    if( FileName != NULL )
    {
        ifstream GraphStream( FileName, ios::in );
        if( GraphStream )
            ReadGraph( GraphStream );
        else
            cerr << "Error opening " << FileName << endl;
    }
}

#if 0
template <class etype>
void
DoubleArray( etype * & array, unsigned int & current_size )
{
    etype *      old_array = array;
    const        min_size  = 5;
    unsigned int new_size  = array ? 2 * current_size : min_size;

    array = new etype [ new_size ];
    for( unsigned int i = 0; i < current_size; i++ )
        array[ i ] = old_array[ i ];
    current_size = new_size;

    delete [ ] old_array;     // Safe even if Old_Array is NULL
}
#endif

// Return the internal number for VertexName
// If VertexName is new, then add it to the dictionary
// and add an entry to graph table, dobuling if needed

template <class NameType, class DistType>
int
Graph<NameType,DistType>::AddNode( const NameType & VertexName )
{
    HashItem<NameType> HashV = VertexName;
    const HashItem<NameType> & Result = VertexMap.Find( HashV );
    
    if( VertexMap.WasFound( ) )
        return Result.Rank;

        // Newly seen vertex
    HashV.Rank = NumVertices;
    VertexMap.Insert( HashV );

    if( NumVertices == Table.Length( ) )
        Table.Double( );
    Table[ NumVertices ].Name = HashV.Name;
    Table[ NumVertices ].Adj = new List< Edge<DistType> >;

    return NumVertices++;
}
// Add the edge ( Source, Dest, Cost ) to the graph
// Source and Dest are internal vertex numbers


template <class NameType, class DistType>
void
Graph<NameType, DistType>::AddInternalEdge( int Source, int Dest, DistType Cost )
{
    ListItr< Edge<DistType> > P = *Table[ Source ].Adj;
    P.Insert( Edge<DistType>( Dest, Cost ) );
}


// Simple main to test the graph algorithms
    
main( )
{
#ifndef NoExceptions
  try
  {
#endif
    Graph<String,double> G( 1e6, "graph.dat" );

    while( G.ProcessRequest( ) )
        ;

    return 0;

#ifndef NoExceptions
  }

  catch( ... )
  {
    cerr << "Out of memory!!" << endl;
    exit( 1 );
  }
#endif
}
