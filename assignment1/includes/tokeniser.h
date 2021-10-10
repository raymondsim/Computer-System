#ifndef ASSIGNMENT_TOKENISER_H
#define ASSIGNMENT_TOKENISER_H

#include <string>

// Interface to the assignment tokeniser
namespace Assignment_Tokeniser
{
    // a shorthand for string
    typedef std::string string ;

    // the ID used to reference a Token - the implementation is hidden
    class _Token ;
    typedef _Token *Token ;

    // ***** BNF *****
    //
    // *** SYNTAX
    // * text literals inside ' '
    // * grouping ( )
    // * character ranges -
    // * alternatives |
    // * 0 or 1 ?
    // * 1 or more +
    // * 0 or more *
    //
    // *** TOKEN RULES
    // * Grammar rule ::= Definition
    //
    // * input ::= token* eoi
    //
    // * token ::= wspace | identifier | number | string | keyword | symbol | eol_comment | adhoc_comment
    //
    // * wspace ::= '\t' | '\n' | '\r' | ' '
    //
    // * identifier  ::= ('a'-'z'|'A'-'Z'|'^') id_letter* '?'?
    //   - id_letter ::= 'a'-'z'|'A'-'Z'|'0'-'9'|'_'|'$'|'.'
    //
    // * number         ::= binary | octal | decimal | hexadecimal
    //   - binary       ::= '0' 'b' bin_digit+ bin_fraction?
    //   - bin_fraction ::= '.' bin_digit*
    //   - bin_digit    ::= '0' | '1'
    //   - octal        ::= '0' oct_digit+ oct_fraction?
    //   - oct_fraction ::= '.' oct_digit*
    //   - oct_digit    ::= '0'-'7'
    //   - decimal      ::= ('0' | decimal19) dec_fraction?
    //   - dec_fraction ::= '.' dec_digit*
    //   - decimal19    ::= ('1'-'9') dec_digit*
    //   - dec_digit    ::= '0'-'9'
    //   - hexadecimal  ::= '0' 'x' hex_digit+ hex_fraction?
    //   - hex_fraction ::= '.' hex_digit*
    //   - hex_digit    ::= '0'-'9'|'A'-'F'
    //
    // * string ::= '"' instring* '"'
    //   - instring ::= ' '|'!'|'#'-'~'|greek
    //   - greek    ::= all unicode code-points from 0x370 to 0x3FF and 0x1F00 to 0x1FFF
    //
    // * keyword ::= 'do'|'for'|'pointer'|'real'|'this' 
    //   - keywords each have their own TokenKind
    //
    // * symbol ::= '@'|';'|':'|'!='|','|'.'|'=='|'<=>'|'{'|'}'|'('|')'|'['|']'|'/'
    //   - symbols each have their own TokenKind
    //
    // * eol_comment  ::= '/' eol_suffix
    //   - eol_suffix ::= '/' eol_char* '\n'
    //   - eol_char   ::= '\t'|'\r'|' '-'~'
    //
    // * adhoc_comment  ::= '/' adhoc_suffix
    //   - adhoc_suffix ::= '*' adhoc_char* '*/'
    //   - adhoc_char   ::= '\t'|'\n'|'\r'|' '-'~'
    //
    // ** PARSING
    // * the functions read_next_char(), next_char_isa() and next_char_mustbe() must be used to parse tokens
    // * all input must be read using the functions read_next_char() or next_char_mustbe()
    // * the function did_not_find_char() must be used to report not finding an expected character or group
    // * do not attempt to parse keywords, they are identified later by the function classify_token()
    // 
    // * PARSING AMBIGUITIES
    // * '/' can start 3 rules: symbol, eol_comment and adhoc_comment
    //   . after parsing the '/' the next character will tell you which of 3 cases you are parsing
    //   . '/' its an eol_comment
    //   . '*' its an adhoc_comment
    //   . anything else its '/'
    // * '0' can start 4 rules, binary, octal, decimal and hexadecimal
    //   . after parsing the '0' the next character will tell you which of 5 cases you are parsing
    //   . 'b' its a binary number
    //   . '0'-'7' its an octal number
    //   . '.' its a decimal number
    //   . 'x' its a hexadecimal number
    //   . anything else its the number '0'
    // * the adhoc comments grammar rules as written do not work with our parsing technique
    //   . adhoc_char includes both '*' and '/' so we need to know when we find the first '*/' combination
    //   . here is an algorithm
    //     1. read adhoc_chars until you find '*'
    //     2. read adhoc_chars until you find something that is not '*'
    //     3. if the next character is not '/' go back to step 1
    //   . this can be programmed using next_char_isa() 
    //   . alternatively the grammar rules could be rewritten as:
    //     adhoc_suffix ::= '*' not_star* '*'+ (not_div not_star* '*'+)* '/'
    //      - not_star  ::= '\t'|'\n'|'\r'|' '-')'|'+'-'~'
    //      - not_div   ::= '\t'|'\n'|'\r'|' '-')'|'+'-'.'|'0'-'~'
    //   . character groups cg_not_star and cg_not_div have been included if you want to support this option
    //
    // ** CHANGES TO TOKEN SPELLING by the function token_spelling()
    // * no spelling changes are required by the MILESTONE TESTS
    // * after parsing and creating a token, modify the token's spelling as follows:
    // * tk_string:
    //    . the '"' and '"' enclosing a string must be removed from the token's spelling
    // * tk_eol_comment:
    //    . the '//' and '\n' enclosing an eol_comment must be removed from the token's spelling
    // * tk_adhoc_comment:
    //    . the '/*' and '*/' enclosing an adhoc_comment must be removed from the token's spelling
    // * tk_number:
    //   . if present, all trailing 0s must be removed from a fraction
    //   . if a fraction has no digits, after removing trailing 0s, the '.' is removed too
    //
    // ** SPECIFIC REQUIREMENTS
    // * there must be a single call to the function keyword_or_identifier()
    // * there must be a single call to the function new_token()
    //
    // ** EXPECTED TEST OUTPUTS
    // * you should consult the expected test outputs if you need help interpreting the token definitions
    //   . 'make Show' can be used to see the names of all files used by each test
    //

0000032.4000000
    // The kinds of token that are recognised
    enum TokenKind
    {
        // MILESTONE TESTS Tokens to be recognised:

        tk_tab,             // '\t'
        tk_newline,         // '\n'
        tk_carriage_return, // '\r'
        tk_space,           // ' '

        tk_identifier,      // identifier
        tk_number,          // number

                            // the single character symbol tokens
        tk_at,              // '@'
        tk_semi,            // ';'
        tk_colon,           // ':'
        tk_comma,           // ','
        tk_dot,             // '.'
        tk_lcb,             // '{'
        tk_rcb,             // '}'
        tk_lrb,             // '('
        tk_rrb,             // ')'
        tk_lsb,             // '['
        tk_rsb,             // ']'
        tk_div,             // '/'

        ////////////////////

        // FINAL TESTS Tokens to be recognised: all of the above and

        tk_string,          // '"' instring* '"', '"' and '"' are not part of the spelling

        tk_eol_comment,     // '//' eol_char* '\n', '//' and '\n' are not part of the spelling
        tk_adhoc_comment,   // '/*' adhoc_char* '*/', '/*' and '*/' are not part of the spelling

        tk_ne,              // '!='
        tk_eq,              // '=='
        tk_spaceship,       // '<=>'

        tk_do,              // 'do'
        tk_for,             // 'for'
        tk_pointer,         // 'pointer'
        tk_real,            // 'real'
        tk_this,            // 'this'

        tk_extra_01,        // extra token kinds for testing / internal use
        tk_extra_02,
        tk_extra_03,
        tk_extra_04,
        tk_extra_05,
        tk_extra_06,
        tk_extra_07,
        tk_extra_08,
        tk_extra_09,
        tk_extra_10,
        tk_extra_11,
        tk_extra_12,
        tk_extra_13,
        tk_extra_14,
        tk_extra_15,
        tk_extra_16,
        tk_extra_17,
        tk_extra_18,
        tk_extra_19,
        tk_extra_20,

        ////////////////////

        tk_eoi,             // end of input reached

        tk_oops,            // for error handling

    } ;

    // ***** the following are implemented in the pre-compiled library files *****

    // return a string representation of the given token kind
    // returns "" if the kind is not recognised
    extern string to_string(TokenKind kind) ;

    // return the TokenKind for the given token
    // it is a fatal error if the token was not returned by a call to next_token()
    extern TokenKind token_kind(Token token) ;

    // return the characters that make up the given token
    // it is a fatal error if the token was not returned by a call to next_token()
    extern string token_spelling(Token token) ;

    // return a string representation of the given token
    // it is a fatal error if the token was not returned by a call to next_token()
    extern string to_string(Token token) ;

    // return a verbose string representation of the given token with additional details
    // it is a fatal error if the token was not returned by a call to next_token()
    extern string to_string_verbose(Token token) ;

    // return a context string for the given token showing its position in the input
    // it is a fatal error if the token was not returned by a call to next_token()
    extern string token_context(Token token) ;

    // to disable use of token_context in tokens tests
    extern void disable_error_context() ;

    // ***** the assignment involves implementing the following functions by completing tokeniser.cpp *****

    // read the next token from standard input
    extern Token read_next_token() ;

}
#endif //ASSIGNMENT_TOKENISER_H
