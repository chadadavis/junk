// Program class interface: evaluate cross-reference
//
// CONSTRUCTION: with (a) open istream or (b) another
//     Program object
//
// ******************PUBLIC OPERATIONS***********************
// void GenerateCrossReference( ) --> Name says it all ...
// ******************ERRORS**********************************
// Error checking on comments and quotes is performed

#include <ctype.h>
#include <fstream.h>

#include "Queue.h"
#include "Iterate.h"
#include "String.h"

#ifdef __GNUC__
    #include "Bst.cpp"
    #include "Iterate.cpp"
    #include "Queue.cpp"
    #include "String.cpp"
#endif

    // Basic object that will be stored in a search tree
struct IdNode
{
    String Word;         // An identifier
    Queue<int> *Lines;   // Lines where it occurs

        // Constructors: default for copy constructor
        // implies default is accepted for destructor
    IdNode( ) : Lines( NULL ) { }
    IdNode( const String & TheWord, int CurrentLine );

        // Ordering operators
    int operator<( const IdNode & Rhs ) const
            { return Word < Rhs.Word; }
    int operator>( const IdNode & Rhs ) const
            { return Word > Rhs.Word; }
    int operator==( const IdNode & Rhs ) const
            { return Word == Rhs.Word; }
};

// Constructor: Note user must delete the queue themselves

IdNode::IdNode( const String & TheWord, int CurrentLine )
                       : Word( TheWord )
{
    Lines = new Queue<int>;
    Lines->Enqueue( CurrentLine );
}

class Program
{
  public:
    Program( istream & Input ) : 
            CurrentLine( 1 ), InputStream( Input ) { }
    ~Program( ) { }  // The text omits the line to fit on the page
    void GenerateCrossReference( );
  private:
    enum CommentType { SlashSlash, SlashStar };

    char Ch;                      // Current character
    IdNode Current;
    int CurrentLine;

        // This part is similar to the Balance symbol checker
    istream & InputStream;        // Reference to the input stream
    int NextChar( );
    void PutBackChar( );
    void SkipComment( CommentType Start );
    void SkipQuote( char QuoteType );
    String GetString( );
    int GetNextSymbol( );
};

// Return indicates if Ch can be part of a C++ identifier

int
IsIdChar( char Ch )
{
    return Ch == '_' || isalnum( Ch );
}

// Return an identifier read from input stream
// First character is already read into Ch

String
Program::GetString( )
{
    static char TmpString[ 1000 ];
    int i;

    for( i = 0; ( TmpString[ i ] = Ch ) && NextChar( ); i++ )
        if( !IsIdChar( Ch ) )
        {
            PutBackChar( );
            break;
        }

    TmpString[ ++i ] = '\0';

    return TmpString;    // Automatic conversion will occur
}


// Reutrn next identifier, skipping comments
// string constants, and character constants
// Place identifier in Current.Word and return 0
// only if end of stream is reached

int
Program::GetNextSymbol( )
{
    while( NextChar( ) )
    {
        if( Ch == '/' )
        {
            if( NextChar( ) )
            {
                if( Ch == '*' )
                    SkipComment( SlashStar );
                else if( Ch == '/' )
                    SkipComment( SlashSlash );
                else
                    PutBackChar( );
            }
        }
        else if( Ch == '\\' )
            NextChar( );
        else if( Ch == '\'' || Ch == '"' )
            SkipQuote( Ch );
        else if( !isdigit( Ch ) && IsIdChar( Ch ) )
        {
            Current.Word = GetString( );
            return 1;
        }
    }
    return 0;       // End of file
}

// Output the cross reference

void
Program::GenerateCrossReference( )
{
    SearchTree<IdNode> TheIdentifiers;

        // Insert identifiers into the search tree
    while( GetNextSymbol( ) )
    {
        const IdNode & ThisNode = TheIdentifiers.Find( Current );
        if( TheIdentifiers.WasFound( ) )
            ThisNode.Lines->Enqueue( CurrentLine );
        else
            TheIdentifiers.Insert( IdNode( Current.Word, CurrentLine ) );
    }

        // Iterate through search tree and output
        // identifiers and their line number
    InOrder<IdNode> Itr = TheIdentifiers;
    for( Itr.First( ); +Itr; ++Itr )
    {
        const IdNode & ThisNode = Itr( );

            // Print identifier and first line where it occurs
        cout << ThisNode.Word << ": " << ThisNode.Lines->GetFront( );
        ThisNode.Lines->Dequeue( );

            // Print all other lines on which it occurs
        while( !ThisNode.Lines->IsEmpty( ) )
        {
            cout << ", " << ThisNode.Lines->GetFront( );
            ThisNode.Lines->Dequeue( );
        }
        cout << '\n';

            // Deallocate the queue
        delete ThisNode.Lines;
    }
}

void
Program::PutBackChar( )
{
    InputStream.putback( Ch );
    if( Ch == '\n' )
        CurrentLine--;
}

int
Program::NextChar( )
{
    if( !InputStream.get( Ch ) )
        return 0;
    if( Ch == '\n' )
        CurrentLine++;
    return 1;
}

// Precondition: We are about to process a comment; have already seen
//                 comment start token
// Postcondition: File pointer will be set immediately after
//                 comment ending token

void
Program::SkipComment( CommentType Start )
{
    if( Start == SlashSlash )
    {
        while( NextChar( ) && ( Ch != '\n' ) )
            ;
        return;
    }

        // Look for */
    int State = 0;    // Number of characters seen in comment ender

    while( NextChar( ) )
    {
        if( State == 1 && Ch == '/' )
            return;
        State = Ch == '*';
    }
}

// Precondition: We are about to process a quote; have already seen
//                   beginning quote.
// Postcondition: File pointer will be set immediately after
//                   matching quote
void
Program::SkipQuote( char QuoteType )
{
    char EndQuote = QuoteType == '"' ? '"' : '\'';

    while( NextChar( ) )
    {
        // If a backslash, skip
        if( Ch == '\\' )
            NextChar( );
        else
        if( Ch == EndQuote )
            return;
        if( Ch == '\n' )
        {
            cout << "Missing closed quote at line " <<
                    CurrentLine << endl;
            return;
        }
    }
}


main( int argc, const char **argv )
{
#ifndef NoExceptions
  try
  {
#endif

    if( argc == 1 )
    {
        Program P( cin );
        P.GenerateCrossReference( );
        return 0;
    }

    while( --argc )
    {
        ifstream Ifp( *++argv, ios::in );
        if( !Ifp )
        {
            cerr << "Can not open " << *argv << endl;
            continue;
        }
        cout << "*********" << *argv << "*********\n";
        Program P( Ifp );
        P.GenerateCrossReference( );
    }


#ifndef NoExceptions
  }

  catch( ... )
  {
    cerr << "Out of memory!!" << endl;
    exit( 1 );
  }
#endif


    return 0;
}
