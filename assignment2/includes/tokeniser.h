#ifndef ASSEMBLER_TOKENISER_H
#define ASSEMBLER_TOKENISER_H

#include <string>
#include <iostream>

// we have one namespace per language, eg Hack_Assembler

namespace Hack_Assembler
{
    // to make our code shorter
    typedef std::string string ;

    // the type of a token returned by the tokeniser
    typedef int Token ;

    // *** BNF syntax for tokens
    // * literals inside ' '
    // * grouping ( )
    // * ASCII ranges -
    // * alternatives |
    // * 0 or 1 [ ]
    // * 0 or more *
    // * 1 or more +
    //

    // Hack Assembly Tokeniser Tokens
    // *** TOKENS
    // * digit    ::= '0'-'9'
    // * digit19  ::= '1'-'9'
    // * letter   ::= 'a'-'z'|'A'-'Z'|'_'|':'|'.'
    // * name     ::= letter ( letter | digit )*
    // * number   ::= '0' | (digit19 digit*)
    //
    // TOKEN: DEFINITION
    // address    ::= ( '@' name ) | ( '@' number )
    // label      ::= '(' name ')'
    // dest       ::= ('A' | 'M' | 'D' | 'MD' | 'AM' | 'AD' | 'AMD') ' '* '='
    // alu        ::= '0' | '1' | '-1' | 'D' | '!D' | '-D' | 'D+1' | 'D-1' |
    //                'A' | '!A' | '-A' | 'A+1' | 'A-1' | 'D+A' | 'D-A' | 'A-D' | 'D&A' | 'D|A' |
    //                'M' | '!M' | '-M' | 'M+1' | 'M-1' | 'D+M' | 'D-M' | 'M-D' | 'D&M' | 'D|M' |
    // jump       ::= ';' ' '* ('JMP' | 'JLT' | 'JLE' | 'JGT' | 'JGE' | 'JEQ' | 'JNE')
    // eol        ::= '\n'
    // NOTES:
    // * Within multi-character alu tokens arbitrary space characters can be present
    // * dest, alu and jmp tokens are all case sensitive, ie amd is not a legal destination, it must be AMD

    // The kinds of token that are recognised
    enum TokenKind
    {
        tk_first = 1000,                // ASM token kinds all > 1000

                                        // destinations, the '=' is not included in the spelling
        tk_dest_A,                      // 'A=', tk_dest
        tk_dest_M,                      // 'M=', tk_dest
        tk_dest_D,                      // 'D=', tk_dest
        tk_dest_MD,                     // 'MD=', tk_dest
        tk_dest_AM,                     // 'AM=', tk_dest
        tk_dest_AD,                     // 'AD=', tk_dest
        tk_dest_AMD,                    // 'AMD=', tk_dest

        tk_alu_0,                       // '0',   tk_alu_op
        tk_alu_1,                       // '1',   tk_alu_op
        tk_alu_sub_1,                   // '-1',  tk_alu_op
        tk_alu_D,                       // 'D',   tk_alu_op
        tk_alu_A,                       // 'A',   tk_alu_op
        tk_alu_not_D,                   // '!D',  tk_alu_op
        tk_alu_not_A,                   // '!A',  tk_alu_op
        tk_alu_sub_D,                   // '-D',  tk_alu_op
        tk_alu_sub_A,                   // '-A',  tk_alu_op
        tk_alu_D_add_1,                 // 'D+1', tk_alu_op
        tk_alu_A_add_1,                 // 'A+1', tk_alu_op
        tk_alu_D_sub_1,                 // 'D-1', tk_alu_op
        tk_alu_A_sub_1,                 // 'A-1', tk_alu_op
        tk_alu_D_add_A,                 // 'D+A', tk_alu_op
        tk_alu_D_sub_A,                 // 'D-A', tk_alu_op
        tk_alu_A_sub_D,                 // 'A-D', tk_alu_op
        tk_alu_D_and_A,                 // 'D&A', tk_alu_op
        tk_alu_D_or_A,                  // 'D|A', tk_alu_op
        tk_alu_M,                       // 'M',   tk_alu_op
        tk_alu_not_M,                   // '!M',  tk_alu_op
        tk_alu_sub_M,                   // '-M',  tk_alu_op
        tk_alu_M_add_1,                 // 'M+1', tk_alu_op
        tk_alu_M_sub_1,                 // 'M-1', tk_alu_op
        tk_alu_D_add_M,                 // 'D+M', tk_alu_op
        tk_alu_D_sub_M,                 // 'D-M', tk_alu_op
        tk_alu_M_sub_D,                 // 'M-D', tk_alu_op
        tk_alu_D_and_M,                 // 'D&M', tk_alu_op
        tk_alu_D_or_M,                  // 'D|M', tk_alu_op

                                        // jump commands, the ';' is not included in the spelling
        tk_jmp,                         // ';JMP', tk_jump
        tk_jlt,                         // ';JLT', tk_jump
        tk_jle,                         // ';JLE', tk_jump
        tk_jgt,                         // ';JGT', tk_jump
        tk_jge,                         // ';JGE', tk_jump
        tk_jeq,                         // ';JEQ', tk_jump
        tk_jne,                         // ';JNE', tk_jump

        tk_name,                        // '@' name, the token spelling does not include the '@' character
        tk_number,                      // '@' number, the token spelling does not include the '@' character
        tk_label,                       // '(' name ')', the token spelling does not include the '(' and ')' characters

        tk_eol,                         // end of line reached, spelling is ""

        tk_eoi,                         // end of input reached or a legal token cannot be formed, spelling is ""

                                        // the following tokens are NOT returned by the tokeniser
        tk_null,                        // 'NULL'

                                        // these are provided to describe groups of tokens
        tk_a_instr,                     // one of tk_name or tk_number


        tk_dest,                        // one of tk_dest_A, tk_dest_M, tk_dest_D, tk_dest_MD, tk_dest_AM, tk_dest_AD, tk_dest_AMD or tk_null

        tk_alu_op,                      // one of tk_alu_A, tk_alu_M, tk_alu_D, tk_alu_0, tk_alu_1, tk_alu_sub_1, tk_alu_not_D, tk_alu_not_A,
                                        //        tk_alu_sub_D, tk_alu_sub_A, tk_alu_D_add_1, tk_alu_A_add_1, tk_alu_D_sub_1, tk_alu_A_sub_1, tk_alu_D_add_A,
                                        //        tk_alu_D_sub_A, tk_alu_A_sub_D, tk_alu_D_and_A, tk_alu_D_or_A, tk_alu_not_M, tk_alu_sub_M, tk_alu_M_add_1,
                                        //        tk_alu_M_sub_1, tk_alu_D_add_M, tk_alu_D_sub_M, tk_alu_M_sub_D, tk_alu_D_and_M or tk_alu_D_or_M

        tk_jump,                        // one of tk_jmp, tk_jlt, tk_jle, tk_jgt, tk_jge, tk_jeq, tk_jne or tk_null

        tk_c_instr,                     // any one from tk_dest or tk_alu_op

        tk_instruction,                 // any one from tk_label, tk_a_instr, tk_c_instr 

        tk_line,                        // any one from tk_instruction or tk_eol

        tk_oops,                        // for error handling
        tk_last                         // the last ASM token value
    } ;

    // return a string representation of the given token kind
    extern string token_kind_to_string(TokenKind kind) ;

    // returns the TokenKind that would cause token_kind_to_string to return s, or tk_oops
    extern TokenKind string_to_token_kind(std::string s) ;

    // read the next token from standard input
    extern Token next_token() ;

    // return the current token, ie the last token returned by next_token()
    extern Token current_token() ;

    // each token has an object that internally records
    // a kind - the TokenKind that describes the token
    // a spelling - the characters that make up the specific token
    // an ivalue - the integer value if the token is a number, otherwise 0
    // a line - the line on which the token starts, the first line of the input is line 1
    // a column - the line position at which the token starts, the first character on a line is in column 1
    // a string representation of the above used to implement a to_string function

    // to assist with error reporting each token has an associated context string
    // the context string each token that includes:
    // - the line before the token,
    // - the line containing the token, and
    // - a line with a ^ marking the token's starting position

    // the following functions return the kind, spelling, line, column, to_string or context for the current token
    extern TokenKind token_kind() ;
    extern string token_spelling() ;
    extern int token_ivalue() ;
    extern int token_line() ;
    extern int token_column() ;
    extern string token_to_string() ;
    extern string token_context() ;

    // the following functions return the kind, spelling, line, column, to_string or context for the given token
    extern TokenKind token_kind(Token token) ;
    extern string token_spelling(Token token) ;
    extern int token_ivalue(Token token) ;
    extern int token_line(Token token) ;
    extern int token_column(Token token) ;
    extern string token_to_string(Token token) ;
    extern string token_context(Token token) ;

    // to assist with parsing, TokenKind has some extra definitions that are never returned as real tokens
    // these extra tokens can be used to check if a token is a member of a particular group
    // for example, tk_statement provides a group of all tokens that can start a statement
    

    // check if we HAVE a current token of the given kind or that belongs to the given grouping?
    extern bool have(TokenKind kind_or_grouping) ;

    // check if the given token is of the given kind or that belongs to the given grouping?
    extern bool have(Token token,TokenKind kind_or_grouping) ;

    // to further assist with parsing, functions have been provided to raise an error if an expected token is missing
    // the MUSTBE functions check a token against an expected kind or grouping
    // if the token matches, the next token is read from the input and the matching token is returned
    // if the token does not match, a fatal_error is called with a suitable message based on the token's context

    // the current token MUSTBE of the given kind or belong to the given grouping?
    extern Token mustbe(TokenKind kind_or_grouping) ;

    // the given token MUSTBE of the given kind or belong to the given grouping?
    extern Token mustbe(Token token,TokenKind kind_or_grouping) ;

}

#endif //ASSEMBLER_TOKENISER_H
