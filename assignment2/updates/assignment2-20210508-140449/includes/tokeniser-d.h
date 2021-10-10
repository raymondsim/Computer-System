#ifndef DISASSEMBLER_TOKENISER_H
#define DISASSEMBLER_TOKENISER_H

#include <string>
#include <iostream>

// we have one namespace per language, eg Hack_Assembler

namespace Hack_Disassembler
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
    // *** MACHINE CODE
    // program    ::= (instr eol?)* eoi
    // *** TOKENS
    // * instr    ::= a-instr | c-instr
    // * a-instr  ::= '0' a-num
    // * a-num    ::= b b b b b b b b b b b b b b b
    // * c-instr  ::= '111' c-a c-alu-op c-dest c-jump
    // * c-a      ::= b
    // * c-alu-op ::= b b b b b b
    // * c-dest   ::= b b b
    // * c-jump   ::= b b b
    // * b        ::= '0' | '1'
    // * eol      ::= '\n' | '\r\n'
    //
    // * NOTE: the only characters permitted on a line are '0' and '1'
    // * NOTE: the line ending may be newline or carriage-return newline
    // * NOTE: any errors are reported as eoi
    //

    // The kinds of token that are recognised
    enum TokenKind
    {
        tk_first = 1500,                // DASM token kinds all > 1500

                                        // destinations, the '=' is not included in the spelling
        tk_a_instr,                     // 'A=', tk_dest
        tk_c_instr,                     // 'A=', tk_dest

        tk_eol,                         // end of line reached, text fields all ""

        tk_eoi,                         // end of input reached or a legal token cannot be formed, text fields all ""

        tk_instruction,                 // either tk_a_instr or tk_c_instr 

        tk_oops,                        // for error handling
        tk_last                         // the last DASM token value
    } ;

    // return a string representation of the given token kind
    extern string token_kind_to_string(TokenKind kind) ;

    // read the next token from standard input
    extern Token next_token() ;

    // return the current token, ie the last token returned by next_token()
    extern Token current_token() ;

    // each token has an object that internally records
    // a kind - the TokenKind that describes the token
    // a spelling - the characters that make up the specific token
    // a line - the line on which the token starts, the first line of the input is line 1
    // a column - the line position at which the token starts, the first character on a line is in column 1
    // its interpretation if it is an A-instruction - 0 if eol or eoi
    // - the integer value of the 16 binary digits (i15 to i0)
    // its interpretation if it is a C-instruction - "" if eol or eoi
    // - the a-bit (i12)
    // - the alu op (i11 to i6)
    // - the destination (i5 to i3)
    // - the jump (i2 to i0)
    // a string representation of the above used to implement a to_string function

    // to assist with error reporting each token has an associated context string
    // the context string each token that includes:
    // - the line before the token,
    // - the line containing the token, and
    // - a line with a ^ marking the token's starting position

    // the following functions return the kind, spelling, line, column, ivalue, abit, aluop, dest, jump, to_string or context for the current token
    extern TokenKind token_kind() ;
    extern string token_spelling() ;
    extern int token_line() ;
    extern int token_column() ;
    extern int token_ivalue() ;
    extern string token_a_bit() ;
    extern string token_alu_op() ;
    extern string token_dest() ;
    extern string token_jump() ;
    extern string token_to_string() ;
    extern string token_context() ;

    // the following functions return the kind, spelling, line, column, ivalue, abit, aluop, dest, jump, to_string or context for the given token
    extern TokenKind token_kind(Token token) ;
    extern string token_spelling(Token token) ;
    extern int token_line(Token token) ;
    extern int token_column(Token token) ;
    extern int token_ivalue(Token token) ;
    extern string token_a_bit(Token token) ;
    extern string token_alu_op(Token token) ;
    extern string token_dest(Token token) ;
    extern string token_jump(Token token) ;
    extern string token_to_string(Token token) ;
    extern string token_context(Token token) ;

    // to assist with parsing, TokenKind has some extra definitions that are never returned as real tokens
    // these extra tokens can be used to check if a token is a member of a particular group
    // for example, tk_instruction provides a group of all tokens that can start an instruction
    

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

#endif //DISASSEMBLER_TOKENISER_H
