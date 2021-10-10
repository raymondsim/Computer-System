#include "iobuffer.h"
#include "tokeniser-extras.h"

// to shorten the code
using namespace std ;
using namespace CS_IO_Buffers ;

namespace Assignment_Tokeniser
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
        case cg_wspace:              // characters that start rule wspace
            switch(ch)
            {
            case '\t':
            case '\n':
            case '\r':
            case ' ':
                return true ;
            default:
                return false ;
            }
                                    // complete a switch statement for each of the following
        case cg_token:              // characters that start rule token
        case cg_identifier:         // characters that start rule identifier
        case cg_id_letter:          // characters that start rule id_letter
        case cg_number:             // characters that start rule number
        case cg_bin_digit:          // characters that start rule bin_digit
        case cg_oct_digit:          // characters that start rule oct_digit
        case cg_decimal:            // characters that start rule decimal
        case cg_decimal19:          // characters that start rule decimal19
        case cg_dec_digit:          // characters that start rule dec_digit
        case cg_hex_digit:          // characters that start rule hex_digit
        case cg_instring:           // characters that start rule instring
        case cg_greek:              // characters that start rule greek
        case cg_eol_char:           // characters that start rule eol_char
        case cg_adhoc_char:         // characters that start rule adhoc_char
        case cg_not_star:           // characters that start rule not_star
        case cg_not_div:            // characters that start rule not_div
        case cg_symbol:             // characters that start rule symbol
        case cg_oops:               //

        default:
            return false ;
        }
    }

    // work out the kind of a parsed token, this may call keyword_or_identifier()
    // the spelling is a valid token or "" if at end of input
    TokenKind classify_spelling(string spelling)
    {
        if ( spelling == "" ) return tk_eoi ;

        return tk_identifier ;
    }

    // work out the correct spelling to use in the Token object being created by new_token()
    // the spelling is a valid token and kind is its kind
    string correct_spelling(TokenKind kind,string spelling)
    {
        if ( spelling == "" ) return "" ;

        return spelling ;
    }

}
