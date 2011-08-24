// THIS PROGRAM IS AN INCOMPLETE
// TIC TAC TOE CLASS
// Comments are minimal; filling in details
// is an exercise


#include <iostream.h>
#include "Hash.h"

#ifdef __GNUC__
    #include "Hash.cpp"
#endif

struct Position
{
    int Board[ 3 ][ 3 ];
    int Value;

    Position( ) { }
    Position( int TheBoard[ 3 ][ 3 ] );

    const Position & operator= ( const Position & Rhs );

    int operator==( const Position & Rhs ) const;
    int operator!=( const Position & Rhs ) const
        { return !( operator==( Rhs ) ); }
};

class TicTacToe
{
  public:
    enum Side { Human, Computer, Empty };
    enum PositionVal { HumanWin, Draw, Unclear, ComputerWin };

    TicTacToe( ) { ClearBoard( ); }   // Constructor

        // Find optimal move
    int ChooseMove( Side S, int & BestRow, int & BestColumn,
        int Alpha = HumanWin, int Beta = ComputerWin, int Depth = 0 );
        // Play move, including checking legality
    int PlayMove( Side S, int Row, int Column );

        // Simple supporting routines
    void ClearBoard( );
    void GetMove( );
    int BoardIsFull( ) const;
    int IsAWin( Side S ) const;

        // Print the board
    friend ostream & operator <<( ostream & Out, const TicTacToe & B );
  private:
    HashTable<Position> Transpositions;

    int Board[ 3 ][ 3 ];

    // Play a move, possible claering a square
    void Place( int Row, int Column, int Piece = Empty )
        { Board[ Row ][ Column ] = Piece; }
    
    int SquareIsEmpty( int Row, int Column ) const
        { return Board[ Row ][ Column ] == Empty; }

    // Compute static value of current position (win, draw, etc.)
    int PositionValue( ) const;
};

int
TicTacToe::PositionValue( ) const
{
    return IsAWin( Computer ) ? ComputerWin :
           IsAWin( Human )    ? HumanWin :
           BoardIsFull( )     ? Draw        : Unclear;
}

int
Position::operator==( const Position & Rhs ) const
{
    for( int i = 0; i < 3; i++ )
        for( int j = 0; j < 3; j++ )
            if( Board[ i ][ j ] != Rhs.Board[ i ][ j ] )
                return 0;

    return 1;
}


const Position &
Position::operator=( const Position & Rhs )
{
    for( int i = 0; i < 3; i++ )
        for( int j = 0; j < 3; j++ )
            Board[ i ][ j ] = Rhs.Board[ i ][ j ];

    Value = Rhs.Value;

    return *this;
}

Position::Position( int TheBoard[ 3 ][ 3 ] )
{
    for( int i = 0; i < 3; i++ )
        for( int j = 0; j < 3; j++ )
            Board[ i ][ j ] = TheBoard[ i ][ j ];
}

unsigned int
Hash( const Position & Key, int TableSize )
{
    unsigned int HashVal = 0;

    for( int i = 0; i < 3; i++ )
        for( int j = 0; j < 3; j++ )
            HashVal = HashVal * 4 ^ Key.Board[ i ][ j ];

    return HashVal % TableSize;
}


int
TicTacToe::ChooseMove( Side S, int & BestRow, int & BestColumn,
        int Alpha, int Beta, int Depth )
{
    Side Opp;             // The other side
    int Reply;            // Value of opponent's reply
    int Dc;               // Placeholder
    int SimpleEval;       // Result of an immediate evaluation
    Position ThisPosition = Board;
    int & Value = ThisPosition.Value;
    static const TableDepth = 5;     // Max depth placed in Trans. table


    if( ( SimpleEval = PositionValue( ) ) != Unclear )
        return SimpleEval;

    if( Depth == 0 )
        Transpositions.MakeEmpty( );
    else if( Depth >= 3 && Depth <= TableDepth )
    {
        const Position & LookupVal =
                Transpositions.Find( ThisPosition );
        if( Transpositions.WasFound( ) )
            return LookupVal.Value;
    }

    if( S == Computer )
    {
        Opp = Human; Value = Alpha;
    }
    else
    {
        Opp = Computer; Value = Beta;
    }

    for( int Row = 0; Row < 3; Row++ )
        for( int Column = 0; Column < 3; Column++ )
            if( SquareIsEmpty( Row, Column ) )
            {
                Place( Row, Column, S );
                Reply = ChooseMove( Opp, Dc, Dc, Alpha, Beta, Depth + 1 );
                Place( Row, Column, Empty );
                if( S == Computer && Reply > Value ||
                    S == Human && Reply < Value )
                {
                    if( S == Computer )
                        Alpha = Value = Reply;
                    else
                        Beta = Value = Reply;
                    
                    BestRow = Row; BestColumn = Column;
                    if( Alpha >= Beta )
                        goto Done;   // Refutation
                }
            }

Done:
    if( Depth <= TableDepth )
        Transpositions.Insert( ThisPosition );
    return Value;
}
