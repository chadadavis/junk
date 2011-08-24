// Program class interface: check for balanced symbols
//
// CONSTRUCTION: with an istream object
// ******************PUBLIC OPERATIONS********************
// int CheckBalance( )   --> Print mismatches
//                           return number of errors

#include <fstream.h>
#include "Stack.h"

#ifdef __GNUC__
    #include "Stack.cpp"
#endif

// Symbol is the class that will be placed on the Stack
struct Symbol
{
    char  Token;
    int   TheLine;
};

class Program
{
  public:
    Program( istream & Input ) : Errors( 0 ),
            CurrentLine( 1 ), InputStream( Input ) { }
    ~Program( ) { }

        // The only publicly visible member function
    int CheckBalance( );
  private:
    enum CommentType { SlashSlash, SlashStar };

    istream & InputStream;        // Reference to the input stream
    char Ch;                      // Current character
    int CurrentLine;              // Current line
    Stack<Symbol> PendingTokens;  // Open symbols pending
    int Errors;                   // Number of errors seen

        // A host of internal routines
    int NextChar( );
    void PutBackChar( );
    void SkipComment( CommentType Start );
    void SkipQuote( char QuoteType );
    char GetNextSymbol( );
    void CheckMatch( const Symbol & OpSym, const Symbol & ClSym );
};

// NextChar sets Ch based on the next character in InputStream
// Adjust CurrentLine if necessary. Return result of get
// PutBackChar puts the character back onto InputStream
// Both routines adjust CurrentLine if necessary

int
Program::NextChar( )
{
    if( !InputStream.get( Ch ) )
        return 0;
    if( Ch == '\n' )
        CurrentLine++;
    return 1;
}

void
Program::PutBackChar( )
{
    InputStream.putback( Ch );
    if( Ch == '\n' )
        CurrentLine--;
}

// Precondition: We are about to process a comment; have already seen
//                 comment start token
// Postcondition: Stream will be set immediately after
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
    Errors++;
    cout << "Unterminated comment!" << endl;
}

// Precondition: We are about to process a quote; have already seen
//                   beginning quote.
// Postcondition: Stream will be set immediately after
//                   matching quote

void
Program::SkipQuote( char QuoteType )
{
    while( NextChar( ) )
    {
        if( Ch == QuoteType )
            return;
        if( Ch == '\n' )
        {
            Errors++;
            cout << "Missing closed quote at line " <<
                    CurrentLine << endl;
            return;
        }
        else if( Ch == '\\' )
            NextChar( );

    }
}

// Return the next opening or closing symbol or 0 (if EOF)
// Skip past comments and character and string constants

char
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
                else if( Ch != '\n' )
                    PutBackChar( );
            }
        }
        else if( Ch == '\'' || Ch == '"' )
            SkipQuote( Ch );
        else if( Ch == '\\' )  // Extra case, not in text
            NextChar( );
        else if( Ch == '(' || Ch == '[' || Ch == '{' ||
             Ch == ')' || Ch == ']' || Ch == '}' )
            return Ch;
    }
    return 0;       // End of file
}

// Print error message for unbalanced symbols
// Return number of errors detected

int
Program::CheckBalance( )
{
    char LastChar;
    Symbol LastSymbol, Match;

    Errors = 0; CurrentLine = 1;
    while( LastChar = GetNextSymbol( ) )
    {
        LastSymbol.Token = LastChar;
        LastSymbol.TheLine = CurrentLine;

        switch( LastChar )
        {
          case '(': case '[': case '{':
            PendingTokens.Push( LastSymbol );
            break;
        
          case ')': case ']': case '}':
            if( PendingTokens.IsEmpty( ) )
            {
                Errors++;
                cout << "Extraneous " << LastChar <<
                    " at line " << CurrentLine << endl;
            }
            else
            {
                Match = PendingTokens.Top( );
                PendingTokens.Pop( );
                CheckMatch( Match, LastSymbol );
            }
            break;

          default: // Can't happen
            break;
        }
    }

    while( !PendingTokens.IsEmpty( ) )
    {
        Errors++;
        Match = PendingTokens.Top( );
        PendingTokens.Pop( );
        cout << "Unmatched " << Match.Token << " at line "
                    << Match.TheLine << endl;
    }
    return Errors;
}

// Print an error message if ClSym does not match OpSym
// Update Error

void
Program::CheckMatch( const Symbol & OpSym, const Symbol & ClSym )
{
    if( OpSym.Token == '(' && ClSym.Token != ')' ||
        OpSym.Token == '[' && ClSym.Token != ']' ||
        OpSym.Token == '{' && ClSym.Token != '}' )
    {
        cout << "Found " << ClSym.Token << " on line " <<
            CurrentLine << "; does not match " << OpSym.Token
            << " at line " << OpSym.TheLine << endl;
        Errors++;
    }
}

// main routine for balanced symbol checker

main( int argc, const char **argv )
{
#ifndef NoExceptions
  try {
#endif

    if( argc == 1 )
    {
        Program P( cin );
        if( P.CheckBalance( ) == 0 )
            cout << "No errors!\n";
        return 0;
    }

    while( --argc )
    {
        ifstream Ifp( *++argv, ios::in );
        if( !Ifp )
        {
            cerr << "Cannot open " << *argv << endl;
            continue;
        }
        cout << *argv << ":\n";
        Program P( Ifp );
        if( P.CheckBalance( ) == 0 )
            cout << "   ...no errors!\n";
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
