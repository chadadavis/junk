// Evaluator clas interface: evaluate infix expression
// Type: Must have usual repertoire of arithmetic operators
//
// CONSTRUCTION: with (a) const char * or (b) another
//     Evaluator object
//
// ******************PUBLIC OPERATIONS***********************
// Type GetValue( )      --> Return value of infix expression
// ******************ERRORS**********************************
// Some error checking is performed


#include <strstream.h>
#include <math.h>
#include "Stack.h"

#ifdef __GNUC__
    #include "Stack.cpp"
#endif

enum Token { EOL, VALUE, OPAREN, CPAREN, EXP, MULT, DIV, PLUS, MINUS };

    // PrecTable matches order of Token enumeration
struct Precedence
{
    int InputSymbol;
    int TopOfStack;
} PrecTable[ ] =
{
    { 0, -1 }, { 0, 0 },       // EOL, VALUE
    { 100, 0 }, { 0, 99 },     // OPAREN, CPAREN
    { 6, 5 },                  // EXP
    { 3, 4 }, { 3, 4 },        // MULT, DIV
    { 1, 2 }, { 1, 2 }         // PLUS, MINUS
};

template <class Type>
class Evaluator
{
  public:
    Evaluator( const char *S ) : Str( S, 256 )
        {  OpStack.Push( EOL ); }

        // The only publicly visible routine
    Type GetValue( );           // Do the evaluation
  private:
    Stack<Token> OpStack;       // Operator stack for conversion
    Stack<Type> PostFixStack;   // Stack for postfix machine

    istrstream Str;             // The character stream
    Type CurrentValue;          // Current operand
    Token LastToken;            // Last token read

        // Internal routines
    Token GetToken( );          // Read next token; put in LastToken
    Type GetTop( );             // Get top of postfix stack
    void BinaryOp( Token );     // Process an operator
    void ProcessToken( );       // Handle LastToken
};

// Public routine that performs the evalaluation
// Examine postfix machine to see if a single result is
// left and if so, return it; otherwise print error

template <class Type>
Type
Evaluator<Type>::GetValue( )
{
    do
    {
        LastToken = GetToken( );
        ProcessToken( );
    } while( LastToken != EOL );


    if( PostFixStack.IsEmpty( ) )
    {
        cerr << "Missing operand!" << endl;
        return 0;
    }

    Type TheResult = PostFixStack.Top( );
    PostFixStack.Pop( );
    if( !PostFixStack.IsEmpty( ) )
        cerr << "Warning: missing operators!" << endl;

    return TheResult;
}

// After token is read, use operator precedence parsing
// algorithm to process it; missing opening parentheses
// are detected here

template <class Type>
void
Evaluator<Type>::ProcessToken( )
{
    Token TopOp;

    switch( LastToken )
    {
      case VALUE:
        PostFixStack.Push( CurrentValue );
        return;

      case CPAREN:
        while( ( TopOp = OpStack.Top( ) ) != OPAREN && TopOp != EOL )
            BinaryOp( TopOp );
        if( TopOp == OPAREN )
            OpStack.Pop( );  // Get rid of opening parentheseis
        else
            cerr << "Missing open parenthesis" << endl;
        break;

      default:    // General operator case
        while( PrecTable[ LastToken ].InputSymbol <=
               PrecTable[ TopOp = OpStack.Top( ) ].TopOfStack )
            BinaryOp( TopOp );
        if( LastToken != EOL )
            OpStack.Push( LastToken );
        break;
    }
}

// Top and Pop the postfix machine stack; return the result
// If stack is empty, print an error message

template <class Type>
Type
Evaluator<Type>::GetTop( )
{
    if( PostFixStack.IsEmpty( ) )
    {
        cerr << "Missing operand" << endl;
        return 0;
    }

    Type Tmp = PostFixStack.Top( );
    PostFixStack.Pop( );
    return Tmp;
}

// A Pow routine for exponentiation

template <class Type>
Type
Pow( const Type & X, const Type & N )
{
    if( X == 0 )
    {
        if( N == 0 )
            cerr << "0^0 is undefined" << endl;
        return 0;
    }
    if( N < 0 )
    {
        cerr << "Negative exponent" << endl;
        return 0;
    }
    if( N == 0 )
        return 1;
    if( N % 2 == 0 )
        return Pow( X * X, N / 2  );
    else
        return X * Pow( X, N - 1 );
}

// Process an operator by taking two items of ff the postfix
// stack, applying the operator, and pushing the result
// Print error if missing closing parenthesis or division by 0

template <class Type>
void
Evaluator<Type>::BinaryOp( Token TopOp )
{
    if( TopOp == OPAREN )
    {
        cerr << "Unbalanced parentheses" << endl;
        OpStack.Pop( );
        return;
    }
    Type Rhs = GetTop( );
    Type Lhs = GetTop( );

    if( TopOp == EXP )
        PostFixStack.Push( Pow( Lhs, Rhs ) );
    else if( TopOp == PLUS )
        PostFixStack.Push( Lhs + Rhs );
    else if( TopOp == MINUS )
        PostFixStack.Push( Lhs - Rhs );
    else if( TopOp == MULT )
        PostFixStack.Push( Lhs * Rhs );
    else if( TopOp == DIV )
        if( Rhs != 0 )
            PostFixStack.Push( Lhs / Rhs );
        else
        {
            cerr << "Division by zero" << endl;
            PostFixStack.Push( Lhs );
        }
    OpStack.Pop( );
}

// Find the next token, skipping blanks, and return it
// For VALUE token, place the processed value in CurrentValue
// Print error message if input is unrecognized

template <class Type>
Token
Evaluator<Type>::GetToken( )
{
    char Ch;

        // Skip blanks
    while( Str.get( Ch ) && Ch == ' ' )
        ;

    if( Str.good( ) && Ch != '\n' && Ch != '\0' )
    {
        switch( Ch )
        {
          case '^': return EXP;
          case '/': return DIV;
          case '*': return MULT;
          case '(': return OPAREN;
          case ')': return CPAREN;
          case '+': return PLUS;
          case '-': return MINUS;

          default:
            Str.putback( Ch );
            if( ( Str >> CurrentValue ) == 0 )
            {
                cerr << "Parse error" << endl;
                return EOL;
            }
            return VALUE;
        }
    }

    return EOL;
}

// Quick and dirty main

main( )
{
    const int LineLength = 80;
    char Str[ LineLength + 1 ];

#ifndef NoExceptions
  try
  {
#endif

    while( cin.getline( Str, LineLength ) )
    {
        Evaluator<int> E( Str );
        cout << E.GetValue( ) << '\n';
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
