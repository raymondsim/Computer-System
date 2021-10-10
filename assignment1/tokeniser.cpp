// AUTHOR: Mong Yuan SIM (a1808469)

// tokeniser implementation for the workshop example language

#include "tokeniser-extras.h"

// to shorten the code
using namespace std;

// we are extending the Assignment_Tokeniser namespace

namespace Assignment_Tokeniser
{
    // token ::= ...
    
    // * wspace ::= '\t' | '\n' | '\r' | ' '
    static void parse_wspace()
    {
        if (next_char_isa(cg_wspace))
            next_char_mustbe(cg_wspace);
        else
            did_not_find_char(cg_wspace);
    }

    //   - id_letter ::= 'a'-'z'|'A'-'Z'|'0'-'9'|'_'|'$'|'.'
    static void parse_id_letter()
    {
        if (next_char_isa(cg_id_letter))
            next_char_mustbe(cg_id_letter);
    }

    // * identifier  ::= ('a'-'z'|'A'-'Z'|'^') id_letter* '?'?
    static void parse_identifier()
    {
        if (next_char_isa(cg_identifier))
            next_char_mustbe(cg_identifier);
        while (next_char_isa(cg_id_letter))
            parse_id_letter();
        if (next_char_isa('?'))
            next_char_mustbe('?');
    }

    //   - bin_digit    ::= '0' | '1'
    static void parse_bin_digit()
    {   
        if(next_char_isa(cg_bin_digit))
            next_char_mustbe(cg_bin_digit);
    }

    //   - bin_fraction ::= '.' bin_digit*
    static void parse_bin_fraction()
    {
        next_char_mustbe('.');
        while (next_char_isa(cg_bin_digit))
            next_char_mustbe(cg_bin_digit);
    }

    //   - binary       ::= '0' 'b' bin_digit+ bin_fraction?
    static void parse_binary()
    {
        next_char_mustbe('b');
        next_char_mustbe(cg_bin_digit);
        while (next_char_isa(cg_bin_digit))
            parse_bin_digit();
        if (next_char_isa(cg_bin_fraction))
            parse_bin_fraction();
    }

    //   - oct_fraction ::= '.' oct_digit*
    static void parse_oct_fraction()
    {
        next_char_mustbe('.');
        while (next_char_isa(cg_oct_digit))
            next_char_mustbe(cg_oct_digit);
    }

    //   - octal        ::= '0' oct_digit+ oct_fraction?
    static void parse_octal()
    {   
        next_char_mustbe(cg_oct_digit);
        while (next_char_isa(cg_oct_digit))
            next_char_mustbe(cg_oct_digit);
        if (next_char_isa(cg_oct_fraction))
            parse_oct_fraction();
    }

    //   - decimal19    ::= ('1'-'9') dec_digit*
    static void parse_decimal19()
    {
        next_char_mustbe(cg_decimal19);
        while (next_char_isa(cg_dec_digit))
            next_char_mustbe(cg_dec_digit);
    }

    //   - dec_fraction ::= '.' dec_digit*
    static void parse_dec_fraction()
    {
        next_char_mustbe('.');
        while (next_char_isa(cg_dec_digit))
            next_char_mustbe(cg_dec_digit);
    }

    //   - decimal      ::= ('0' | decimal19) dec_fraction?
    static void parse_decimal()
    {
        parse_decimal19();
        if (next_char_isa(cg_dec_fraction))
            parse_dec_fraction();
    }

    //   - hex_digit    ::= '0'-'9'|'A'-'F'
    static void parse_hex_digit()
    {   
        if(next_char_isa(cg_hex_digit))
            next_char_mustbe(cg_hex_digit);
    }

    //   - hex_fraction ::= '.' hex_digit*
    static void parse_hex_fraction()
    {
        next_char_mustbe('.');
        while (next_char_isa(cg_hex_digit))
            next_char_mustbe(cg_hex_digit);
    }

    //   - hexadecimal  ::= '0' 'x' hex_digit+ hex_fraction?
    static void parse_hexadecimal()
    {
        next_char_mustbe('x');
        next_char_mustbe(cg_hex_digit);
        while (next_char_isa(cg_hex_digit))
            parse_hex_digit();
        if (next_char_isa(cg_hex_fraction))
            parse_hex_fraction();
    }

    // * string ::= '"' instring* '"'
    static void parse_string()
    {
        next_char_mustbe('"');
        while (next_char_isa(cg_instring))
            next_char_mustbe(cg_instring);
        next_char_mustbe('"');
    }

    //to parse adhoc suffix
    static void parse_adhoc_suffix_extra()
    {
        next_char_mustbe(cg_not_div);
        while (next_char_isa(cg_not_star))
            next_char_mustbe(cg_not_star);
        next_char_mustbe('*');
        while (next_char_isa('*'))
            next_char_mustbe('*');
    }

    //   - adhoc_suffix ::= '*' adhoc_char* '*/'
    static void parse_adhoc_suffix()
    {
        next_char_mustbe('*');
        while (next_char_isa(cg_not_star))
            next_char_mustbe(cg_not_star);
        while (next_char_isa('*'))
            next_char_mustbe('*');
        while (next_char_isa(cg_not_div))
            parse_adhoc_suffix_extra();
        next_char_mustbe('/');
    }

    // * adhoc_comment  ::= '/' adhoc_suffix
    static void parse_adhoc_comment()
    {
        if (next_char_isa('*'))
            parse_adhoc_suffix();
    }

    // * symbol ::= '@'|';'|':'|'!='|','|'.'|'=='|'<=>'|'{'|'}'|'('|')'|'['|']'|'/'
    //   - symbols each have their own TokenKind
    static void parse_symbol()
    {
        if (next_char_isa('!'))
        {
            next_char_mustbe('!');
            if (next_char_isa('='))
                next_char_mustbe('=');
        }
        else if (next_char_isa('<'))
        {
            next_char_mustbe('<');
            next_char_mustbe('=');
            next_char_mustbe('>');
        }
        else if (next_char_isa('='))
        {
            next_char_mustbe('=');
            next_char_mustbe('=');
        }
        else if (next_char_isa('/'))
        {
            next_char_mustbe('/');
            if (next_char_isa('/'))
            {
                next_char_mustbe('/');
                while (next_char_isa(cg_eol_char))
                    next_char_mustbe(cg_eol_char);
                next_char_mustbe('\n');
            }
            else if (next_char_isa('*'))
            {
                parse_adhoc_comment();
            }
        }
        else
        {
            next_char_mustbe(cg_symbol);
        }
    }

    //parse number with four kind of numbers
    static void parse_number()
    {
        if (next_char_isa(cg_decimal19))
            parse_decimal();
        else if (next_char_isa('0'))
        {
            next_char_mustbe('0');
            if (next_char_isa('b'))
                parse_binary();
            else if (next_char_isa(cg_oct_digit))
                parse_octal();
            else if (next_char_isa('.'))
                parse_dec_fraction();
            else if (next_char_isa('x'))
                parse_hexadecimal();
        }
        else
            did_not_find_char(cg_number);
    }

    //parse token with different kind of  inputs
    static void parse_token()
    {
        if (next_char_isa(cg_wspace))
            parse_wspace();
        else if (next_char_isa(cg_identifier))
            parse_identifier();
        else if (next_char_isa(cg_number))
            parse_number();
        else if (next_char_isa(cg_string))
            parse_string();
        else if (next_char_isa(cg_symbol))
            parse_symbol();
        else if (next_char_isa(EOF)) /* do nothing once at end of input */
            ;
        else
            did_not_find_char(cg_token);
    }

    // parse the next token in the input and return a new
    // Token object that describes its kind and spelling
    // Note: you must not call new_token() anywhere else in your program
    // Note: you should not modify this function
    Token read_next_token()
    {
        parse_token();

        return new_token();
    }
}
