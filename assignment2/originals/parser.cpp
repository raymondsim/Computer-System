// convert Hack Assembly Language into an abstract syntax tree
#include "iobuffer.h"
#include "tokeniser.h"
#include "abstract-syntax-tree.h"

// to make out programs a bit neater
using namespace std ;

using namespace CS_IO_Buffers ;
using namespace Hack_Assembler ;

// grammar to be parsed:
// a_program ::= line* eoi
// line ::= instruction? eol
// instruction ::= a_label | a_instr | c_instr
// a_label ::= label
// a_instr ::= a_instr_name | a_instr_number
// a_instr_name ::= name
// a_instr_number ::= number
// c_instr ::= dest? alu_op jump?
// Tokens: label, name, number, dest, alu_op, jump, eol, eoi

// forward declare parsing functions - one per rule
ast parse_program() ;
ast parse_instruction() ;
ast parse_a_label() ;
ast parse_a_instr() ;
ast parse_a_instr_name() ;
ast parse_a_instr_number() ;
ast parse_c_instr() ;

// Note: always read one token after the one recognised

// a_program ::= line* tk_eoi
// line ::= instruction? tk_eol
ast parse_program()
{
    vector<ast> instructions ;

    // we stop when we see the EOI token
    // this may be end of input or an error
    // we cannot tell so treat both as end of input
    // every new assembler instruction we find is appended to the_program
    while ( have(tk_line) )
    {
        if ( have(tk_instruction) ) instructions.push_back(parse_instruction()) ;
        mustbe(tk_eol) ;
    }
    mustbe(tk_eoi) ;

    return create_program(instructions) ;
}

/*****************   REPLACE THE FOLLOWING CODE  ******************/

// instruction ::= tk_label | A-instr | C-instr
ast parse_instruction()
{
    // inspect the token to see what it might start
    if ( have(tk_label) ) return parse_a_label() ;
    if ( have(tk_a_instr) ) return parse_a_instr() ;
    if ( have(tk_c_instr) ) return parse_c_instr() ;
    mustbe(tk_instruction) ;

    return nullptr ;
}

// tk_label is the token for '(' label ')'
ast parse_a_label()
{
    string label = "" ;

    next_token() ;              // replace this with code to parse a label

    return create_label(label) ;
}

// A-instr ::= tk_name | tk_number
ast parse_a_instr()
{
    // inspect the token to see what it might start
    if ( have(tk_name) ) return parse_a_instr_name() ;
    if ( have(tk_number) ) return parse_a_instr_number() ;
    mustbe(tk_a_instr) ;

    return nullptr ;
}

// tk_name is the token for '@' name
ast parse_a_instr_name()
{
    string name = "" ;

    next_token() ;              // replace this with code to parse '@'name

    return create_a_name(name) ;
}

// tk_number is the token for '@' number
ast parse_a_instr_number()
{
    int ivalue = 0 ;

    next_token() ;              // replace this with code to parse '@'number

    return create_a_instruction(ivalue) ;
}

// C-instr ::= dest? alu_op jump?
ast parse_c_instr()
{
    string dest = "NULL" ;
    string alu_op = "" ;
    string jump = "NULL" ;

    next_token() ;              // replace this with code to parse a C instruction that starts with a register

    return create_c_instruction(dest,alu_op,jump) ;
}

/*****************        DOWN TO HERE         ******************/


// main program
int main(int argc,char **argv)
{
    // initialise the tokeniser by reading the first token
    next_token() ;

    // parse a class and print the abstract syntax tree as XML
    ast_print_as_xml(parse_program(),2) ;

    // flush the output and any errors
    print_output() ;
    print_errors() ;
}

