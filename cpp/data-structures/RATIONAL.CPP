#include "Rational.h"

// N is guaranteed non-negative

IntType
Gcd1( const IntType & N, const IntType & M )
{
    if( N % M == 0 )
        return M;
    else
        return Gcd1( M, N % M );
}

IntType
Gcd( const IntType & M, const IntType & N )
{
    if( M > 0 )
        return Gcd1( N, M );
    else
        return Gcd1( N, -M );
}

void
Rational::FixSigns( )
{
    if( Denom < 0 )
    {
        Denom = -Denom;
        Numer = -Numer;
    }
}

void
Rational::Reduce( )
{
    IntType D = 1;

    if( Denom != 0 && Numer != 0 )
        D = Gcd( Numer, Denom );

    if( D > 1 )
    {
        Numer /= D;
        Denom /= D;
    }
}

const Rational &
Rational::operator=( const Rational & Rhs )
{
    if( this != &Rhs )
    {
        Numer = Rhs.Numer;
        Denom = Rhs.Denom;
    }
    return *this;
}

const Rational &
Rational::operator+=( const Rational & Rhs )
{
    Numer = Numer * Rhs.Denom + Rhs.Numer * Denom;
    Denom = Denom * Rhs.Denom;
    Reduce( );

    return *this;
}

const Rational &
Rational::operator-=( const Rational & Rhs )
{
    Numer = Numer * Rhs.Denom - Rhs.Numer * Denom;
    Denom = Denom * Rhs.Denom;
    Reduce( );

    return *this;
}

const Rational &
Rational::operator*=( const Rational & Rhs )
{
    IntType NewNumer = Numer * Rhs.Numer;
    IntType NewDenom = Denom * Rhs.Denom;

    Numer = NewNumer;
    Denom = NewDenom;
    Reduce( );

    return *this;
}

const Rational &
Rational::operator/=( const Rational & Rhs )
{
    IntType NewNumer = Numer * Rhs.Denom;
    IntType NewDenom = Denom * Rhs.Numer;

    Numer = NewNumer;
    Denom = NewDenom;

    FixSigns( );
    Reduce( );

    return *this;
}

Rational
Rational::operator+( const Rational & Rhs ) const
{
    Rational Answer( *this );
    Answer += Rhs;
    return Answer;
}

Rational
Rational::operator-( const Rational & Rhs ) const
{
    Rational Answer( *this );
    Answer -= Rhs;
    return Answer;
}

Rational
Rational::operator*( const Rational & Rhs ) const
{
    Rational Answer( *this );
    Answer *= Rhs;
    return Answer;
}

Rational
Rational::operator/( const Rational & Rhs ) const
{
    Rational Answer( *this );
    Answer /= Rhs;
    return Answer;
}

int
Rational::operator==( const Rational & Rhs ) const
{
    return Numer * Rhs.Denom == Denom * Rhs.Numer;
}

int
Rational::operator!=( const Rational & Rhs ) const
{
    return Numer * Rhs.Denom != Denom * Rhs.Numer;
}
int
Rational::operator<=( const Rational & Rhs ) const
{
    return Numer * Rhs.Denom <= Denom * Rhs.Numer;
}

int
Rational::operator<( const Rational & Rhs ) const
{
    return Numer * Rhs.Denom < Denom * Rhs.Numer;
}

int
Rational::operator>=( const Rational & Rhs ) const
{
    return Numer * Rhs.Denom >= Denom * Rhs.Numer;
}

int
Rational::operator>( const Rational & Rhs ) const
{
    return Numer * Rhs.Denom > Denom * Rhs.Numer;
}

const Rational &
Rational::operator++( )
{
    Numer += Denom;
    return *this;
}

Rational
Rational::operator++( int )
{
    Rational Tmp = *this;
    Numer += Denom;
    return Tmp;
}

const Rational &
Rational::operator--( )
{
    Numer -= Denom;
    return *this;
}

Rational
Rational::operator--( int )
{
    Rational Tmp = *this;
    Numer -= Denom;
    return Tmp;
}

int
Rational::operator!( ) const
{
    return !Numer;
}

const Rational &
Rational::operator+( ) const
{
    return *this;
}

Rational
Rational::operator-( ) const
{
    return Rational( -Numer, Denom );
}

istream &
operator>>( istream & In, Rational & Value )
{
    In >> Value.Numer;
    
    char Ch;
    In >> Ch;
    if( Ch == '/' )
    {
        In >> Value.Denom;
        Value.FixSigns( );
        Value.Reduce( );
    }
    else
    {
        Value.Denom = 1;
        In.putback( Ch );
    }

    return In;
}

ostream &
operator<<( ostream & Out, const Rational & Value )
{
    if( Value.Denom != 0 )
    {
        Out << Value.Numer;
        if( Value.Denom != 1 )
            Out << '/' << Value.Denom;
        return Out;
    }

    if( Value.Numer == 0 )
        Out << "indeterminate";
    else
    {
        if( Value.Numer < 0 )
            Out << '-';
        Out << "infinity";
    }
    return Out;
}
