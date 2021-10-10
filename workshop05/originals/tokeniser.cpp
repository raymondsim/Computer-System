// tokeniser implementation for the workshop example language

#include "tokeniser-extras.h"

// to shorten the code
using namespace std ;

// we are extending the Workshop_Tokeniser namespace

namespace Workshop_Tokeniser
{
    // wspace ::= '\t' | '\n' | '\r' | ' '
    static void parse_wspace()
    {
    }

    // identifier ::= ('a'-'z'|'A'-'Z')('a'-'z'|'A'-'Z'|'0'-'9')*
    static void parse_identifier()
    {
    }

    // integer ::= '0' | (('1'-'9')('0'-'9')*)
    static void parse_integer()
    {
    }

    // op ::= '+' | '-' | '*' | '/'
    static void parse_op()
    {
    }

    // these 4 functions are commented out to avoid unused function warnings
    // if you choose not to use them, delete them

    // lt_le := '<' | '<='
    //static void parse_lt_le()
    //{
    //}

    // assign_eq ::= '=' | '=='
    //static void parse_assign_eq()
    //{
    //}

    // not_ne ::= '!' | '!='
    //static void parse_not_ne()
    //{
    //}

    // gt_ge ::= '>' | '>='
    //static void parse_gt_ge()
    //{
    //}

    // varop ::= lt_le | assign_eq | not_ne | gt_ge
    // ch is '<', '=', '!', or '>'
    static void parse_varop()
    {
    }

    // parse a single character symbol
    // symbol ::= '@' | '{' | '}' | '(' | ')' | ':' | ';' | '.' | ',' | '"'
    static void parse_symbol()
    {
    }

    // token ::= wspace | identifier | integer | op | varop | symbol
    static void parse_token()
    {
        if ( next_char_isa(cg_wspace) ) parse_wspace() ; else
        if ( next_char_isa(cg_identifier) ) parse_identifier() ; else
        if ( next_char_isa(cg_integer) ) parse_integer() ; else
        if ( next_char_isa(cg_op) ) parse_op() ; else
        if ( next_char_isa(cg_varop) ) parse_varop() ; else
        if ( next_char_isa(cg_symbol) ) parse_symbol() ; else
        if ( next_char_isa(EOF) ) ; else
        did_not_find_char(cg_token) ;
    }

    // parse the next token in the input and return a new
    // Token object that describes its kind and spelling
    // Note: you must not call new_token() anywhere else in your program
    //
    Token read_next_token()
    {
        parse_token() ;

        return new_token() ;
    }
}

