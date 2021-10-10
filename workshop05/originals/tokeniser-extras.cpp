// convert Text into Tokens wrapped up in XML
#include "iobuffer.h"
#include "tokeniser-extras.h"
#include <unordered_map>
#include <iostream>
#include <vector>

// to shorten the code
using namespace std ;
using namespace CS_IO_Buffers ;

namespace Workshop_Tokeniser
{
    // check if a char matches another char or is a member of a character group
    // eg char_isa('3',cg_digit) returns true
    bool char_isa(int ch,int cg)
    {
        // single character groups use their character as their value
        // do not add them to the switch statement

        if ( ch == cg ) return true ;

        switch(cg)
        {
        case cg_token:               // characters that start rule token
            switch(ch)
            {
            default:
                return false ;
            }
        case cg_wspace:              // characters that start rule wspace
            switch(ch)
            {
            default:
                return false ;
            }
        case cg_identifier:          // characters that start rule identifier
            switch(ch)
            {
            default:
                return false ;
            }
        case cg_letter:              // characters that start rule letter
            switch(ch)
            {
            default:
                return false ;
            }
        case cg_alnum:               // characters that start rule alnum
            switch(ch)
            {
            default:
                return false ;
            }
        case cg_integer:             // characters that start rule integer
            switch(ch)
            {
            default:
                return false ;
            }
        case cg_digit19:             // characters that start rule digit19
            switch(ch)
            {
            default:
                return false ;
            }
        case cg_digit:               // characters that start rule digit
            switch(ch)
            {
            default:
                return false ;
            }
        case cg_op:                  // characters that start rule op
            switch(ch)
            {
            default:
                return false ;
            }
        case cg_varop:               // characters that start rule varop
            switch(ch)
            {
            default:
                return false ;
            }
        case cg_symbol:              // characters that start rule symbol
            switch(ch)
            {
            default:
                return false ;
            }
        default:
            return false ;
        }
    }

    // work out the kind of a parsed token, this may call keyword_or_identifier()
    // the spelling is a valid token
    TokenKind classify_spelling(string spelling)
    {
        if ( spelling == "" ) return tk_eoi ;

        return tk_identifier ;
    }

    // work out the correct spelling to use in the Token object being created by new_token()
    // the spelling is a valid token and kind is its kind
    string correct_spelling(TokenKind kind,string spelling)
    {
        return spelling ;
    }

}
