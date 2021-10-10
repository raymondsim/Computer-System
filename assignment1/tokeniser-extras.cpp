

#include "iobuffer.h"
#include "tokeniser-extras.h"

// to shorten the code
using namespace std;
using namespace CS_IO_Buffers;

namespace Assignment_Tokeniser
{
    // check if a char matches another char or is a member of a character group
    // eg char_isa('3',cg_digit) returns true
    bool char_isa(int ch, int cg)
    {
        // single character groups use their character as their value
        // do not add them to the switch statement
        if (ch == cg)
            return true;

        switch (cg)
        {
        // characters that start rule wspace
        case cg_wspace: 
            switch (ch)
            {
            case '\t':
            case '\n':
            case '\r':
            case ' ':
                return true;
            default:
                return false;
            }
        // complete a switch statement for each of the following
        // characters that start rule token
        case cg_token: 
            switch (ch)
            {
            default:
                return false;
            }
        // characters that start rule identifier
        case cg_identifier: 
            switch (ch)
            {
            case 'a' ... 'z':
            case 'A' ... 'Z':
            case '^':
                return true;
            default:
                return false;
            }

        // characters that start rule id_letter
        case cg_id_letter: 
            switch (ch)
            {
            case 'a' ... 'z':
            case 'A' ... 'Z':
            case '0' ... '9':
            case '_':
            case '$':
            case '.':
                return true;
            default:
                return false;
            }

        // characters that start rule number
        case cg_number: 
            switch (ch)
            {
            case '0' ... '9':
                return true;
            default:
                return false;
            }

        // characters that start rule bin_digit
        case cg_bin_digit: 
            switch (ch)
            {
            case '0':
            case '1':
                return true;
            default:
                return false;
            }

        // characters that start rule oct_digit
        case cg_oct_digit: 
            switch (ch)
            {
            case '0' ... '7':
                return true;
            default:
                return false;
            }

        // characters that start rule decimal
        case cg_decimal: 
            switch (ch)
            {
            case '0' ... '9':
                return true;
            }

        // characters that start rule decimal19
        case cg_decimal19: 
            switch (ch)
            {
            case '1' ... '9':
                return true;
            default:
                return false;
            }

        // characters that start rule dec_digit
        case cg_dec_digit: 
            switch (ch)
            {
            case '0' ... '9':
                return true;
            default:
                return false;
            }

        // characters that start rule hex_digit
        case cg_hex_digit: 
            switch (ch)
            {
            case '0' ... '9':
            case 'A' ... 'F':
                return true;
            default:
                return false;
            }

        // characters that start rule instring
        case cg_instring: 
            switch (ch)
            {
            case ' ':
            case '!':
            case '#' ... '~':
            case L'\u0370' ... L'\u03FF':
            case L'\u1F00' ... L'\u1FFF':
                return true;
            default:
                return false;
            }

        // characters that start rule greek
        case cg_greek: 
            switch (ch)
            {
            case L'\u0370' ... L'\u03FF':
            case L'\u1F00' ... L'\u1FFF':
                return true;
            default:
                return false;
            }

        // characters that start rule eol_char
        case cg_eol_char: 
            switch (ch)
            {
            case '\t':
            case '\r':
            case ' ' ... '~':
                return true;
            default:
                return false;
            }

        // characters that start rule adhoc_char
        case cg_adhoc_char: 
            switch (ch)
            {
            case '\t':
            case '\n':
            case '\r':
            case ' ' ... '~':
                return true;
            default:
                return false;
            }

        // characters that start rule not_star
        case cg_not_star: 
            switch (ch)
            {
            case '\t':
            case '\n':
            case '\r':
            case ' ' ... ')':
            case '+' ... '~':
                return true;
            default:
                return false;
            }

        // characters that start rule not_div
        case cg_not_div: 
            switch (ch)
            {
            case '\t':
            case '\n':
            case '\r':
            case ' ' ... ')':
            case '+' ... '.':
            case '0' ... '~':
                return true;
            default:
                return false;
            }

        // characters that start rule symbol
        case cg_symbol: 
            switch (ch)
            {
            case '@':
            case ';':
            case ':':
            case '!':
            case ',':
            case '.':
            case '=':
            case '<':
            case '{':
            case '}':
            case '(':
            case ')':
            case '[':
            case ']':
            case '/':
                return true;
            default:
                return false;
            }
        case cg_oops:
        default:
            return false;
        }
    }

    // work out the kind of a parsed token, this may call keyword_or_identifier()
    // the spelling is a valid token or "" if at end of input
    TokenKind classify_spelling(string spelling)
    {
        if (spelling == "")
            return tk_eoi;
        if (spelling == "do")
            return tk_do;
        if (spelling == "for")
            return tk_for;
        if (spelling == "pointer")
            return tk_pointer;
        if (spelling == "real")
            return tk_real;
        if (spelling == "this")
            return tk_this;

        switch (spelling[0])
        {
        case '=':
            if (spelling[1] == '=')
                return tk_eq;
        case '<':
            if (spelling[1] == '=' && spelling[2] == '>')
                return tk_spaceship;
        case '"':
            if (spelling[spelling.length() - 1] == '"')
                return tk_string;
        case 'a' ... 'z':
        case 'A' ... 'Z':
            return keyword_or_identifier(spelling);
        case '^':
            return tk_identifier;
        case '!':
            if (spelling[1] == '=')
            {
                return tk_ne;
            }
        case '0' ... '9':
            return tk_number;
        case '\t':
            return tk_tab;
        case '\n':
            return tk_newline;
        case '\r':
            return tk_carriage_return;
        case ' ':
            return tk_space;
        case '@':
            return tk_at;
        case ';':
            return tk_semi;
        case ':':
            return tk_colon;
        case ',':
            return tk_comma;
        case '.':
            return tk_dot;
        case '{':
            return tk_lcb;
        case '}':
            return tk_rcb;
        case '(':
            return tk_lrb;
        case ')':
            return tk_rrb;
        case '[':
            return tk_lsb;
        case ']':
            return tk_rsb;
        case '/':
            if (spelling[1] == '/')
            {
                return tk_eol_comment;
            }
            else if (spelling[1] == '*')
            {
                return tk_adhoc_comment;
            }
            else
            {
                return tk_div;
            }
        }
        return tk_identifier;
    }

    // work out the correct spelling to use in the Token object being created by new_token()
    // the spelling is a valid token and kind is its kind
    string correct_spelling(TokenKind kind, string spelling)
    {
        //fixing eol_comment
        if (kind == tk_eol_comment)
        {
            for (int i = 0; i < spelling.length(); i++)
            {
                spelling[i] = spelling[i + 2];
            }
            spelling.pop_back();
            spelling.pop_back();
            for (int i = 0; i < spelling.length(); i++)
            {
                if (spelling[i] == '\n')
                {
                    spelling[i] = spelling[i + 1];
                    spelling.pop_back();
                }
            }
        }

        //fixing adhoc_comment
        if (kind == tk_adhoc_comment)
        {
            for (int i = 0; i < spelling.length(); i++)
            {
                spelling[i] = spelling[i + 2];
            }
            spelling.pop_back();
            spelling.pop_back();
            spelling.pop_back();
            spelling.pop_back();
        }

        //fixing quotation mark for string
        if (kind == tk_string)
        {
            for (int i = 0; i < spelling.length(); i++)
            {
                spelling[i] = spelling[i + 1];
            }
            spelling.pop_back();
            spelling.pop_back();
        }

        //fixing extra dot and zeroes from numbers
        int dot_index;
        bool dot_exist = false;

        if (kind == tk_number)
        {
            //fix dot with zeroes behind
            //find if dot exist
            for (int i = 0; i < spelling.length(); i++)
            {
                if (spelling[i] == '.')
                {
                    dot_index = i;
                    dot_exist = true;
                }
            }
            if (dot_exist)
            {
                int initial_spelling_length = spelling.length();
                for (int i = dot_index; i < initial_spelling_length; i++)
                {
                    if (spelling.back() == '0' || spelling.back() == '.')
                    {
                        spelling.pop_back();
                    }
                }
            }
        }
        return spelling;
    }
}