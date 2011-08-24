#ifndef __RATIONAL
#define __RATIONAL

// Rational class interface: support operations for rationals
//
// CONSTRUCTION: with (a) no initializer or (b) an integer
//     that specifies the numerator, or (c) two integers
//     specifying numerator and denominator, or
//     (d) another Rational
//
// ******************PUBLIC OPERATIONS*********************
// =, +=, -=, /=, *=      --> Usual assignment
// +, -, *, /             --> Usual binary arithmetic
// <, <=, >, >=, ==, !=   --> Usual relational and equality
// << and >>              --> Input and output
// double LongDecimal( )  --> Return double equivalent

#include <iostream.h>

typedef long IntType;

class Rational
{
  public:
        // Constructors
    Rational( const IntType & Numerator = 0 ) :
        Numer( Numerator ), Denom( 1 ) { }
    Rational( const IntType & Numerator,
              const IntType & Denominator ) :
        Numer( Numerator ), Denom( Denominator )
            { FixSigns( ); Reduce( ); }
    Rational( const Rational & Rhs ) :
        Numer( Rhs.Numer ), Denom( Rhs.Denom ) { }

        // Destructor
    ~Rational( ) { }

        // Assignment Operators
    const Rational & operator= ( const Rational & Rhs );
    const Rational & operator+=( const Rational & Rhs );
    const Rational & operator-=( const Rational & Rhs );
    const Rational & operator/=( const Rational & Rhs );
    const Rational & operator*=( const Rational & Rhs );

        // Mathematical Binary Operators
    Rational operator+( const Rational & Rhs ) const;
    Rational operator-( const Rational & Rhs ) const;
    Rational operator/( const Rational & Rhs ) const;
    Rational operator*( const Rational & Rhs ) const;

        // Relational and Equality Operators
    int operator< ( const Rational & Rhs ) const;
    int operator<=( const Rational & Rhs ) const;
    int operator> ( const Rational & Rhs ) const;
    int operator>=( const Rational & Rhs ) const;
    int operator==( const Rational & Rhs ) const;
    int operator!=( const Rational & Rhs ) const;

        // Unary Operators
    const Rational & operator++( );            // Prefix
    Rational operator++( int );                // Postfix
    const Rational & operator--( );            // Prefix
    Rational operator--( int );                // Postfix
    const Rational & operator+( ) const;
    Rational operator-( ) const;
    int operator!( ) const;

        // Member Function
    double LongDecimal( ) const   // Do the division
        { return double( Numer ) / double( Denom ); }

        // Friends of the class: privacy is waived for these
    friend ostream & operator<<
            ( ostream & Out, const Rational & Value );
    friend istream & operator>>
            ( istream & In,  Rational & Value );
  private:
    IntType Numer;             // The numerator
    IntType Denom;             // The denominator

    void FixSigns( );          // Ensures Denom >= 0
    void Reduce( );            // Ensures lowest form
};
#endif
