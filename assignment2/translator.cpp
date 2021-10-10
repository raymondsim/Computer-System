//AUTHOR: MONG YUAN SIM A1808469
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

int value = 16; // this is to keep track of variables set by user
int return_value; // this is value used to returned from search_or_add_name function


//this function convert an int to a 16-bit binary string
// this will return a string for later use
string dec_to_binary_str(int x)
{
    string output = "0000000000000000";
    int counter = 15;

    for (int i=0; x>0; i++)
    {
        // cout<<x%2<<endl;
        char temp = x%2 + '0';
        output[counter] = temp;
        counter--;
        x = x/2;
    }
    return output + '\n';
}

//search for a name and if not exist, add it to table
string search_or_add_a_name(ast instruction, isymbols predefined)
{

    if ( isymbols_insert(predefined, get_a_name_unresolved(instruction), value ))
    {
        isymbols_insert(predefined, get_a_name_unresolved(instruction), value);
        int search_result = value;
        value += 1;
        return dec_to_binary_str(search_result);
    }else
    {
        int search_result = isymbols_lookup(predefined, get_a_name_unresolved(instruction));
        return dec_to_binary_str(search_result);
    }
}

// this function creates symbol table for predefined commands
isymbols predefined_symbol_table()
{
    isymbols predefined = isymbols_create();
    isymbols_insert(predefined, "SP", 0);
    isymbols_insert(predefined, "LCL", 1);
    isymbols_insert(predefined, "ARG", 2);
    isymbols_insert(predefined, "THIS", 3);
    isymbols_insert(predefined, "THAT", 4);
    isymbols_insert(predefined, "R0", 0);
    isymbols_insert(predefined, "R1", 1);
    isymbols_insert(predefined, "R2", 2);
    isymbols_insert(predefined, "R3", 3);
    isymbols_insert(predefined, "R4", 4);
    isymbols_insert(predefined, "R5", 5);
    isymbols_insert(predefined, "R6", 6);
    isymbols_insert(predefined, "R7", 7);
    isymbols_insert(predefined, "R8", 8);
    isymbols_insert(predefined, "R9", 9);
    isymbols_insert(predefined, "R10", 10);
    isymbols_insert(predefined, "R11", 11);
    isymbols_insert(predefined, "R12", 12);
    isymbols_insert(predefined, "R13", 13);
    isymbols_insert(predefined, "R14", 14);
    isymbols_insert(predefined, "R15", 15);
    isymbols_insert(predefined, "SCREEN", 16384);
    isymbols_insert(predefined, "KBD", 24576);

    return predefined;
}

// create jump table for later use
symbols jump_symbol_table()
{

    symbols jump = symbols_create();
    symbols_insert(jump, "NULL", "000");
    symbols_insert(jump, "JGT", "001");
    symbols_insert(jump, "JEQ", "010");
    symbols_insert(jump, "JGE", "011");
    symbols_insert(jump, "JLT", "100");
    symbols_insert(jump, "JNE", "101");
    symbols_insert(jump, "JLE", "110");
    symbols_insert(jump, "JMP", "111");

    return jump;

}

//this function creates symbol table for alu or comp instructions
symbols alu_symbol_table() 
{

    symbols alu=symbols_create();
    symbols_insert(alu, "0", "0101010");
    symbols_insert(alu, "1", "0111111");
    symbols_insert(alu, "-1", "0111010");
    symbols_insert(alu, "D", "0001100");
    symbols_insert(alu, "A", "0110000");
    symbols_insert(alu, "M", "1110000");
    symbols_insert(alu, "!D", "0001101");
    symbols_insert(alu, "!A", "0110001");
    symbols_insert(alu, "!M", "1110001");
    symbols_insert(alu, "-D", "0001111");
    symbols_insert(alu, "-A", "0110011");
    symbols_insert(alu, "-M", "1110011");
    symbols_insert(alu, "D+1", "0011111");
    symbols_insert(alu, "A+1", "0110111");
    symbols_insert(alu, "M+1", "1110111");
    symbols_insert(alu, "D-1", "0001110");
    symbols_insert(alu, "A-1", "0110010");
    symbols_insert(alu, "M-1", "1110010");
    symbols_insert(alu, "D+A", "0000010");
    symbols_insert(alu, "D+M", "1000010");
    symbols_insert(alu, "D-A", "0010011");
    symbols_insert(alu, "D-M", "1010011");
    symbols_insert(alu, "A-D", "0000111");
    symbols_insert(alu, "M-D", "1000111");
    symbols_insert(alu, "D&A", "0000000");
    symbols_insert(alu, "D&M", "1000000");
    symbols_insert(alu, "D|A", "0010101");
    symbols_insert(alu, "D|M", "1010101");

    return alu;
}

// this function creates symbol table 
symbols dest_symbol_table() 
{

    symbols dest=symbols_create();
    symbols_insert(dest,"NULL","000");
    symbols_insert(dest,"M","001");
    symbols_insert(dest,"D","010");
    symbols_insert(dest,"MD","011");
    symbols_insert(dest,"A","100");
    symbols_insert(dest,"AM","101");
    symbols_insert(dest,"AD","110");
    symbols_insert(dest,"AMD","111");
    
    return dest;
}


// this function provides an example of how to walk an abstract syntax tree constructed by ast_parse_xml()
void walk_program(ast the_program, isymbols predefined)
{   
    int location_counter = 0;
    // the_program contains a vector of instructions
    int ninstructions = size_of_program(the_program) ;
    for ( int i = 0 ; i < ninstructions ; i++ )
    {
        ast instruction = get_program(the_program,i) ;
        switch ( ast_node_kind(instruction) )
        {
        case ast_a_name:
        case ast_a_instruction:
        case ast_c_instruction:
            location_counter += 1;
            break ;

        case ast_label:
        //when it reads a label, insert it to table if it's not in it initially
            if ( isymbols_insert(predefined,get_label_name(instruction),location_counter) ){
                isymbols_insert(predefined,get_label_name(instruction),location_counter);
                break;
            }
            break;

        default:
            fatal_error(0,"// bad node - expected ast_label,ast_a_name,ast_a_instruction or ast_c_instruction\n") ;
            break ;
        }
    }
}

//this function will walk through second time and ignore labels and
//output 16 bit binary 
void second_walk_program(ast the_program, isymbols predefined, symbols dest_symbol_table, symbols jump_symbol_table, symbols alu)
{   
    //initialise ``
    string dest_str;
    string comp_str;
    string jump_str;

    // the_program contains a vector of instructions
    int ninstructions = size_of_program(the_program) ;
    for ( int i = 0 ; i < ninstructions ; i++ )
    {
        ast instruction = get_program(the_program,i) ;
        switch ( ast_node_kind(instruction) )
        {
        case ast_label:
            break ;
        case ast_a_name:
            write_to_output(search_or_add_a_name(instruction, predefined));
            break;
        case ast_a_instruction:
            write_to_output(dec_to_binary_str(get_a_instruction_value(instruction)));
            break;
        case ast_c_instruction:
            comp_str = symbols_lookup(alu, get_c_instruction_alu(instruction));
            dest_str = symbols_lookup(dest_symbol_table, get_c_instruction_dest(instruction));
            jump_str = symbols_lookup(jump_symbol_table, get_c_instruction_jump(instruction));
            write_to_output("111" + comp_str + dest_str + jump_str + '\n');
            break;

        default:
            fatal_error(0,"// bad node - expected ast_label,ast_a_name,ast_a_instruction or ast_c_instruction\n") ;
            break ;
        }
    }
}

// translate an abstract syntax tree representation of Hack Assembly language into Hack machine code
static void asm_translator(ast the_program)
{
    isymbols predefined = predefined_symbol_table();
    symbols alu = alu_symbol_table();
    symbols jump = jump_symbol_table();
    symbols dest = dest_symbol_table();
    walk_program(the_program, predefined);
    second_walk_program(the_program, predefined, dest, jump, alu);
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



string a_name(ast instruction){
    string test = get_a_name_unresolved(instruction);
    if (isymbols_lookup(instruction_table, ))
}
