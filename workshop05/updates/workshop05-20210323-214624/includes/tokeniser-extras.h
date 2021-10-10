#ifndef WORKSHOP_TOKENISER_EXTRAS_H
#define WORKSHOP_TOKENISER_EXTRAS_H

#include "tokeniser.h"

// Interface for the workshop tokeniser
namespace Workshop_Tokeniser
{
    // character groupings to make parsing code simpler
    // for each grammar rule we define the group of characters that could start the rule
    // but not for keywords which are not explicitly parsed
    // for use with next_char_isa(), next_char_mustbe() and did_not_find_char()
    enum Char:int
    {
        cg_min = 0x70000000,    // make all character groups start outside uni-code range

        cg_token,               // characters that start rule token
        cg_wspace,              // characters that start rule wspace
        cg_identifier,          // characters that start rule identifier
        cg_letter,              // characters that start rule letter
        cg_alnum,               // characters that start rule alnum
        cg_integer,             // characters that start rule integer
        cg_digit19,             // characters that start rule digit19
        cg_digit,               // characters that start rule digit
        cg_op,                  // characters that start rule op
        cg_varop,               // characters that start rule varop
        cg_symbol,              // characters that start rule symbol
        cg_oops,                //

                                // single character groups can use the character as their value
        cg_label = '(',         // single character '('
        cg_lt_le = '<',         // single character '<'
        cg_assign_eq = '=',     // single character '='
        cg_not_ne = '!',        // single character '!'
        cg_gt_ge = '>',         // single character '>'
        cg_eoi = EOF            // single character EOF
    } ;

    // ***** the following are implemented in the pre-compiled lib/libcs_w05.a object file *****

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

    // work out the kind of a parsed token, this calls keyword_or_identifier() if a token looks like an identifier
    extern TokenKind classify_spelling(string spelling) ;

    // work out the correct spelling to use in the Token object being created by new_token()
    extern string correct_spelling(TokenKind kind,string spelling) ;

    // check if a char matches another char or is a member of a character group
    // eg char_isa(':',cg_symbol) returns true
    // eg char_isa('-',cg_identifier) returns false
    extern bool char_isa(int ch,int cg) ;
}

#endif //WORKSHOP_TOKENISER_EXTRAS_H
