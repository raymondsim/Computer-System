// convert an abstract syntax tree for Hack Assembly language into machine code
#include "iobuffer.h"
#include "symbols.h"
#include "tokeniser.h"
#include "abstract-syntax-tree.h"

// to simplify the code
using namespace std ;
using namespace CS_IO_Buffers ;
using namespace CS_Symbol_Tables ;
using namespace Hack_Assembler ;

// this function provides an example of how to walk an abstract syntax tree constructed by ast_parse_xml()
// it visits each instruction or label in order and accesses all fields in each tree node
void walk_program(ast the_program)
{
    // the_program contains a vector of instructions
    int ninstructions = size_of_program(the_program) ;
    for ( int i = 0 ; i < ninstructions ; i++ )
    {
        ast instruction = get_program(the_program,i) ;

        string temp ;
        switch(ast_node_kind(instruction))
        {
        case ast_label:
            temp = get_label_name(instruction) ;
            break ;
        case ast_a_name:
            temp = get_a_name_unresolved(instruction) ;
            break ;
        case ast_a_instruction:
            temp = get_a_instruction_value(instruction) ;
            break ;
        case ast_c_instruction:
            temp = get_c_instruction_dest(instruction) ;
            temp = get_c_instruction_alu(instruction) ;
            temp = get_c_instruction_jump(instruction) ;
            break ;
        default:
            fatal_error(0,"// bad node - expected ast_label,ast_a_name,ast_a_instruction or ast_c_instruction\n") ;
            break ;
        }
    }
}

// translate an abstract syntax tree representation of Hack Assembly language into Hack machine code
static void asm_translator(ast the_program)
{
}

// main program
int main(int argc,char **argv)
{
    // error messages
    config_errors(iob_buffer) ;

    // parse abstract syntax tree and pass to the translator
    asm_translator(ast_parse_xml()) ;

    // flush output and errors
    print_output() ;
    print_errors() ;
}
