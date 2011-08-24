#include "String.h"
#include "Exception.h"

char *String::NullString = (char *) "";

String::String( const char *Value )
{
    if( Value == NULL )      // Use "" if Value is NULL
    {
        GetBuffer( 0 );
        Buffer[ 0 ] = '\0';
    }
    else
    {
        GetBuffer( strlen( Value ) );
        strcpy( Buffer, Value );
    }
}

String::String( const String & Value )
{
    GetBuffer( strlen( Value.Buffer ) );
    strcpy( Buffer, Value.Buffer );
}

const String &
String::operator=( const String & Rhs )
{
    const int Len = strlen( Rhs.Buffer );

    if( this != &Rhs )       // Don't copy to yourself
    {
        if( Len >= BufferLen )
        {
            if( BufferLen != -1 )
                delete [ ] Buffer;
            GetBuffer( Len );
        }
        strcpy( Buffer, Rhs.Buffer );
    }

    return *this;
}

const String &
String::operator=( const char * Rhs )
{
    if( Rhs == NULL )
        Rhs = "";

    const int Len = strlen( Rhs );

    if( Len >= BufferLen )
    {
        if( BufferLen != -1 )
            delete [ ] Buffer;
        GetBuffer( Len );
    }
    strcpy( Buffer, Rhs );

    return *this;
}

istream &
operator >> ( istream & In, String & Value )
{
    static char Str[ 1024 ];

    In >> Str;
    Value = Str;
    return In;
}

ostream &
operator << ( ostream & Out, const String & Value )
{
    return Out << Value.Buffer;
}

char &
String::operator[ ]( int Index )
{
    EXCEPTION( Index < 0 || Index > strlen( Buffer ),
                "Index out of range" );
    return Buffer[ Index ];
}

char
String::operator[ ]( int Index ) const
{
    EXCEPTION( Index < 0 || Index > strlen( Buffer ),
                "Index out of range" );
    return Buffer[ Index ];
}
