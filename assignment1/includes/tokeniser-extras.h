#ifndef ASSIGNMENT_TOKENISER_EXTRAS_H
#define ASSIGNMENT_TOKENISER_EXTRAS_H

#include "tokeniser.h"

// Interface to extra support function for the tokeniser
namespace Assignment_Tokeniser
{
    // character groupings to make parsing code simpler
    // for each grammar rule we define a group for all the characters that could start the rule
    // but not for keywords which are not explicitly parsed
    // single character groups use the character as their value
    // for use with next_char_isa(), next_char_mustbe() and did_not_find_char()
    enum Char:int
    {
        cg_min = 0x70000000,    // make all character groups start outside the legal uni-code range

        cg_token,               // characters that start rule token
        cg_wspace,              // characters that start rule wspace
        cg_identifier,          // characters that start rule identifier
        cg_id_letter,           // characters that start rule id_letter
        cg_number,              // characters that start rule number
        cg_bin_digit,           // characters that start rule bin_digit
        cg_oct_digit,           // characters that start rule oct_digit
        cg_decimal,             // characters that start rule decimal
        cg_decimal19,           // characters that start rule decimal19
        cg_dec_digit,           // characters that start rule dec_digit
        cg_hex_digit,           // characters that start rule hex_digit
        cg_instring,            // characters that start rule instring
        cg_greek,               // characters that start rule greek
        cg_eol_char,            // characters that start rule eol_char
        cg_adhoc_char,          // characters that start rule adhoc_char
        cg_not_star,            // characters that start rule not_star
        cg_not_div,             // characters that start rule not_div
        cg_symbol,              // characters that start rule symbol
        cg_oops,                //

                                // single character groups use char as their value
                                // defined last so they do not get mixed up with other groups
        cg_binary = '0',        // single character '0'
        cg_bin_fraction = '.',  // single character '.'
        cg_octal = '0',         // single character '0'
        cg_oct_fraction = '.',  // single character '.'
        cg_dec_fraction = '.',  // single character '.'
        cg_hexadecimal = '0',   // single character '0'
        cg_hex_fraction = '.',  // single character '.'
        cg_string = '"',        // single character '"'
        cg_eol_comment = '/',   // single character '/'
        cg_eol_suffix = '/',    // single character '/'
        cg_adhoc_comment = '/', // single character '/'
        cg_adhoc_suffix = '*',  // single character '*'
        cg_eoi = EOF            // single character EOF
    } ;

    // ***** the following are implemented in the pre-compiled object files *****

    // ***** Input functions *****
 
    // read the next character if not at the end of input
    // all input must be read using this function
    // the first call initialises the tokeniser
    extern void read_next_char() ;

    // ***** Parsing functions *****
 
    // report an error and exit because the next character in the input is not expected?
    extern void did_not_find_char(int expected) ;

    // return true if the next character is expected, otherwise false
    // this is implemented by calling char_isa()
    extern bool next_char_isa(int expected) ;

    // if the next character is expected call read_next_char(),
    // otherwise call did_not_find_char()
    extern void next_char_mustbe(int expected) ;

    // ***** Token functions *****
 
    // if s is a keyword it returns its token kind otherwise it returns tk_identifier
    extern TokenKind keyword_or_identifier(string s) ;

    // create a new Token object of the given kind using all characters parsed since the last call
    // it calls classify_spelling() to work out the kind of token that was parsed
    // it calls correct_spelling() to work out the spelling of the token
    extern Token new_token() ;

    // ***** the following are implemented in the tokeniser-extras.cpp file *****

    // check if a char matches another char or is a member of a character group
    // eg char_isa(':',cg_symbol) returns true
    // eg char_isa('-',cg_identifier) returns false
    extern bool char_isa(int ch,int cg) ;

    // work out the kind of a parsed token, this calls keyword_or_identifier() if a token looks like an identifier
    extern TokenKind classify_spelling(string spelling) ;

    // work out the correct spelling to use in the Token object being created by new_token()
    extern string correct_spelling(TokenKind kind,string spelling) ;

}

#endif //ASSIGNMENT_TOKENISER_EXTRAS_H
