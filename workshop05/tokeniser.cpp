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
        if(next_char_isa(cg_wspace)){
            next_char_mustbe(cg_wspace);
        }else{
            did_not_find_char(cg_wspace);
        }
    }

    // identifier ::= ('a'-'z'|'A'-'Z')('a'-'z'|'A'-'Z'|'0'-'9')*
    static void parse_identifier()
    {
        // next_char_mustbe(cg_identifier);
        // while(next_char_isa(cg_alnum)){read_next_char();};
        next_char_mustbe(cg_letter);
        while(next_char_isa(cg_alnum))next_char_mustbe(cg_alnum);
    }

    // integer ::= '0' | (('1'-'9')('0'-'9')*)
    static void parse_integer()
    {   
        if(next_char_isa('0')){
            read_next_char();
        }else{
            while(next_char_isa(cg_integer)){
                read_next_char();
            }
        }
        // next_char_mustbe(cg_digit19);
        // while(next_char_isa(cg_digit19)){read_next_char();};
    }

    // op ::= '+' | '-' | '*' | '/'
    static void parse_op()
    {   
        next_char_mustbe(cg_op);
        // if(next_char_isa(cg_op)){read_next_char();};
    }

    // these 4 functions are commented out to avoid unused function warnings
    // if you choose not to use them, delete them

    // lt_le := '<' | '<='
    // static void parse_lt_le()
    // {
    // if(next_char_isa('=')){
    //     read_next_char();
    // }else{

    // }
    // }

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

    static void parse_label(){
        // next_char_mustbe('(');
        // // next_char_mustbe(cg_identifier);
        // read_next_char();
        // next_char_mustbe(')');

        next_char_mustbe('(');
        parse_identifier();
        next_char_mustbe(')');
        
    }

    // varop ::= lt_le | assign_eq | not_ne | gt_ge
    // ch is '<', '=', '!', or '>'
    static void parse_varop()
    {
        read_next_char();
        if(next_char_isa('=')) read_next_char();
        // while(next_char_isa(cg_varop)){read_next_char();};
    }

    // parse a single character symbol
    // symbol ::= '@' | '{' | '}' | '(' | ')' | ':' | ';' | '.' | ',' | '"'
    static void parse_symbol()
    {
        next_char_mustbe(cg_symbol);
        // while(next_char_isa(cg_symbol)){read_next_char();};
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
        if ( next_char_isa(cg_label)) parse_label(); else
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

