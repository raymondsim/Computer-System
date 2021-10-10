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
                case '\t':
                case '\n':
                case '\r':
                case ' ':
                    return true;
            default:
                return false ;
            }
        case cg_identifier:          // characters that start rule identifier
            switch(ch)
            {
                case 'a' ... 'z':
                case 'A' ... 'Z':
                // case '0' ... '9':
                    return true;
            default:
                return false ;
            }
        case cg_letter:              // characters that start rule letter
            switch(ch)
            {
                case 'a' ... 'z':
                case 'A' ... 'Z':
                    return true;
            default:
                return false ;
            }
        case cg_alnum:               // characters that start rule alnum
            switch(ch)
            {
                case '0' ... '9':
                case 'a' ... 'z':
                case 'A' ... 'Z':
                    return true;
            default:
                return false ;
            }
        case cg_integer:             // characters that start rule integer
            switch(ch)
            {
                case '0' ... '9':
                    return true;
            default:
                return false ;
            }
        case cg_digit19:             // characters that start rule digit19
            switch(ch)
            {
                case '1' ... '9':
                    return true;
            default:
                return false ;
            }
        case cg_digit:               // characters that start rule digit
            switch(ch)
            {
                case '0' ... '9':
                    return true;
            default:
                return false ;
            }
        case cg_op:                  // characters that start rule op
            switch(ch)
            {
                case '+':
                case '-':
                case '*':
                case '/':
                    return true;
            default:
                return false ;
            }
        case cg_varop:               // characters that start rule varop
            switch(ch)
            {
                case '<':
                case '=':
                case '>':
                case '!':
                    return true;
            default:
                return false ;
            }
        case cg_symbol:              // characters that start rule symbol
            switch(ch)
            {
                case '@':
                case '{':
                case '}':
                // case '(':
                case ')':
                case ':':
                case ';':
                case '.':
                case ',':
                case '"':
                    return true;
            }
        // case cg_label:
        //     switch(ch)
        //     {
        //         case '(':
        //             return true;
            
        //     default:
        //         return false ;
        //     }
        default:
            return false ;
        }
    }

    // work out the kind of a parsed token, this may call keyword_or_identifier()
    // the spelling is a valid token
    TokenKind classify_spelling(string spelling)
    {
        if (spelling == "") return tk_eoi;
        if (spelling == "==") return tk_eq;
        if (spelling == ">=") return tk_ge;
        if (spelling == "<=") return tk_le;
        if (spelling == "!=") return tk_ne;

        switch(spelling[0]){
            case '\n':
                return tk_newline;
            case 'A' ... 'Z':
                return keyword_or_identifier(spelling);
            case 'a' ... 'z':
                return keyword_or_identifier(spelling);
            case '0' ... '9':
                return tk_integer;
            case ' ':
                return tk_space;
            case '\t':
                return tk_tab;
            case '\r':
                return tk_carriage_return;
            case '+':
                return tk_add;
            case '-':
                return tk_sub;
            case '*':
                return tk_times;
            case '/':
                return tk_divide;
            case '<':
                return tk_lt;
            case '=':
                return tk_assign;
            case '!':
                return tk_not;
            case '>':
                return tk_gt;
            case '@':
                return tk_at;
            case '{':
                return tk_lcb;
            case '}':
                return tk_rcb;
            case ':':
                return tk_colon;
            case ';':
                return tk_semi;
            case '.':
                return tk_dot;
            case ',':
                return tk_comma;
            case '"':
                return tk_dquote;
            case '(':
                return tk_label; 
            case ')':
                return tk_label; 

            default:
                break;
        }
        return tk_identifier;
    }

    // work out the correct spelling to use in the Token object being created by new_token()
    // the spelling is a valid token and kind is its kind
    string correct_spelling(TokenKind kind,string spelling)
    {
        if(kind==tk_label){
            return spelling.substr(1, spelling.length()-2);
        }else{
            return spelling;
        }

        // if(spelling[0]=='('){
        //     for(int i=0;i<spelling.size();i++){
        //         spelling[i] = spelling[i+1];
        //     }
        //     spelling.pop_back();
        //     spelling.pop_back();
        // }
        // return spelling;
        
    }
}

