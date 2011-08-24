#include <fstream.h>
#include <string.h>        // For strlen, strcpy, etc.
#include "String.h"        // For our String class
#include "Vector.h"

#ifdef __GNUC__
    #include "String.cpp"
    #include "BinSrch1.cpp"      // Binary search
#else
    #include "BinSrch1.h"
#endif

static const int MaxWordLength = 24;  // Longest word allowed

int
IsPrefix( const String & Prefix, const String & Word )
{
    for( int i = 0; i < Prefix.Length( ); i++ )
        if( Prefix[ i ] != Word[ i ] )
            return 0;

    return 1;
}

// Puzzle class interface: solve word search puzzle
//
// CONSTRUCTION: with (a) no initialzier
// ******************PUBLIC OPERATIONS******************
// int SolvePuzzle( )   --> Print all words found in the
//                          puzzle; return number of matches
// PC USERS: May need to adjust MaxWords

class Puzzle
{
  public:
    Puzzle( );
    int SolvePuzzle( );
    enum { MaxRows = 64, MaxColumns = 64, MaxWords = 100000 };
  private:
    int Rows;
    int Columns;
    int NumEntries;
    char TheBoard[ MaxRows ][ MaxColumns ];
    Vector<String> TheWords;
    ifstream PuzzleStream;
    ifstream WordStream;

    void OpenFile( const char *Message, ifstream & InFile );
    void ReadPuzzle( );
    void ReadWords( );
    int SolveDirection( int BaseRow, int BaseCol,
                        int RowDelta, int ColDelta );
};

// Routine to solve the word serach puzzle
// Performs checks in all eight directions

int
Puzzle::SolvePuzzle( )
{
    int Matches = 0;
    for( int R = 0; R < Rows; R++ )
        for( int C = 0; C < Columns; C++ )
            for( int RD = -1; RD <= 1; RD++ )
                for( int CD = -1; CD <= 1; CD++ )
                    if( RD != 0 || CD != 0 )
                        Matches += SolveDirection( R, C, RD, CD );

    return Matches;
}

// String operators = and += for a character as
// parameter are supposed to be done as an exercise
// Rather than supply the answer, we code member
// functions (rather sloppily and inefficiently).

const String &
String::operator=( char Rhs )
{
    char RStr[ 2 ] = { Rhs, '\0' };
    return operator=( RStr );
}


const String &
String:: operator+=( char Rhs )
{
    char Tmp[ MaxWordLength ];

    strcpy( Tmp, Buffer );
    int i = strlen( Tmp );

    Tmp[ i++ ] = Rhs;
    Tmp[ i ] = '\0';

    return operator=( Tmp );
}

// Search the grid from a starting point and direction

int
Puzzle::SolveDirection( int BaseRow, int BaseCol,
                        int RowDelta, int ColDelta )
{
    static String CharSequence;
    int NumMatches = 0;
    int SearchResult;

    CharSequence = TheBoard[ BaseRow ][ BaseCol ];
    
    for( int i = BaseRow + RowDelta, j = BaseCol + ColDelta;
            i >= 0 && j >= 0 && i < Rows && j < Columns;
                i += RowDelta, j += ColDelta )
    {
        CharSequence += TheBoard[ i ][ j ];
        SearchResult = BinarySearch( TheWords, CharSequence, NumEntries );

        if( !IsPrefix( CharSequence, TheWords[ SearchResult ] ) )
            break;

        if( TheWords[ SearchResult ] == CharSequence )
        {
            NumMatches++;
            cout << "Found " << CharSequence << " at " <<
                BaseRow << " " << BaseCol << " to " <<
                i << " " << j << "\n";
        }
    }
    return NumMatches;
}

// Print a prompt and open a file
// retry until open is successful

void
Puzzle::OpenFile( const char *Message, ifstream & InFile )
{
    const int FileNameLen = 1024;
    char Name[ FileNameLen + 1 ];

    do
    {
        cout << Message << ": ";
        cin.getline( Name, FileNameLen );
        InFile.open( Name, ios::in );
    } while( !InFile );
}
        
// Constructor for Puzzle class
// Prompts for and reads puzzle and dictionary files

Puzzle::Puzzle( ) : TheWords( MaxWords )
{
    OpenFile( "Enter puzzle file", PuzzleStream );
    OpenFile( "Enter dictionary name", WordStream );
    ReadPuzzle( );
    ReadWords( );
}

// Routine to read the grid
// Checks to ensure that the grid is rectangular
// Checks to make sure that capacity is not exceeded is omitted 

void
Puzzle::ReadPuzzle( )
{
    char FirstLine[ MaxColumns + 1 ];

    if( !PuzzleStream.getline( FirstLine, MaxColumns ) )
    {
        Rows = 0;
        return;
    }
    Columns = strlen( FirstLine );
    for( int i = 0; i < Columns; i++ )
        TheBoard[ 0 ][ i ] = FirstLine[ i ];

    for( Rows = 1; PuzzleStream.getline( FirstLine, MaxColumns ); Rows++ )
    {
        if( strlen( FirstLine ) != Columns )
            cerr << "Puzzle is not rectangular" << endl;

        for( int i = 0; i < Columns; i++ )
            TheBoard[ Rows ][ i ] = FirstLine[ i ];
    }
}

// Routine to read the dictionary
// Error message is printed if dictionary is not sorted
// Check is made to avoid exceeding MaxWords

void
Puzzle::ReadWords( )
{
    NumEntries = 0;
    while( WordStream >> TheWords[ NumEntries ] )
    {
        if( NumEntries != 0 &&
            TheWords[ NumEntries ] < TheWords[ NumEntries - 1 ] )
        {
            cerr << "Dictionary is not sorted... skipping" << endl;
            continue;
        }
        else if( ++NumEntries >= MaxWords )
            break;
    }
    if( !WordStream.eof( ) )
        cerr << "Warning: unread data - increase MaxWords" << endl;
}

// Cheap main

main( )
{
    Puzzle P;
    P.SolvePuzzle( );

    return 0;
}
