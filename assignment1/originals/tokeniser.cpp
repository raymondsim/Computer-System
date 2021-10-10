// tokeniser implementation for the workshop example language

#include "tokeniser-extras.h"

// to shorten the code
using namespace std ;

// we are extending the Assignment_Tokeniser namespace

namespace Assignment_Tokeniser
{
    // token ::= ...
    static void parse_token()
    {
        if ( next_char_isa(EOF) ) /* do nothing once at end of input */ ; else
        did_not_find_char(cg_token) ;
    }

    // parse the next token in the input and return a new
    // Token object that describes its kind and spelling
    // Note: you must not call new_token() anywhere else in your program
    // Note: you should not modify this function
    //
    Token read_next_token()
    {
        parse_token() ;

        return new_token() ;
    }
}

