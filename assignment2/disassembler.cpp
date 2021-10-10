//AUTHOR: MONG YUAN SIM A1808469

// convert Hack Machine Code into Assembly Language
#include "iobuffer.h"
#include "symbols.h"
#include "tokeniser-d.h"
#include <vector>
#include <iostream>

// to make out programs a bit neater
using namespace std ;

using namespace CS_IO_Buffers ;
using namespace CS_Symbol_Tables ;
using namespace Hack_Disassembler ;

static symbols predefined_table = symbols_create();
static symbols alu_table = symbols_create();
static symbols dest_table = symbols_create();
static symbols jump_table = symbols_create();
static symbols symbol_table = symbols_create();
static vector<int> label;


// This program must disassemble Hack Machine Code and output the equivalent Hack Assembly Language.
//
// A-instructions must be translated into symbolic form using the following rules:
//
// 1. IF symbols is false
//    THEN
//     - the A-instruction is output in numeric form.
//
// 2. IF an A-instruction is followed by a C-instruction and the jump bits of the C-instruction are not "000"
//    THEN
//     - IF the value of the A-instruction is the address an instruction in the program
//       THEN
//        - the instruction must be allocated a label and the A-instruction must use this label
//        - the first labelled location in memory must be labelled L0, the next L1 and so on
//
// 3. IF an A-instruction is followed by a C-instruction that reads or writes memory
//    THEN
//     - IF the address has a predefined name
//       THEN
//        - the A-instruction must use the predefined name
//        - where there is a choice, use SP, LCL, ARG, THIS and THAT in preference to R0, R1, R2 R3 and R4
//    - IF the address is the address of an existing variable location
//      THEN
//       - the A-instructon must use the variable's name
//    - IF the address is the address of the next free variable location
//      THEN
//       - the A-instructon must use the next free variable's name and
//       - the address of the next free variable must be incremented by 1
//       - the first variable name allocated will be for address 16 and will be called v_0
//       - the second variable name allocated will be for address 17 and will be called v_1
//       - and so on
//
// 4. ELSE the A-instruction is output in numeric form.
//
// C-instructions must be translated using the following rules:
//
// 1. IF the destination bits are "000"
//    THEN
//     - do not output a destination component for the C-instruction
//
// 2. IF the ALU operation is not recognised
//   THEN
//    - replace the ALU operation with: "< ** UNDEFINED ALU OPERATION ** >"
//
// Do not output a jump component for a C-instruction if it the jump bits are "000"
// 3. IF the jump bits are "000"
//    THEN
//     - do not output a jump componentfor the C-instruction
//

//create table for references later use for lookup
//this includes alu, jump, and other tables
static void create_table()
{
    symbols_insert(predefined_table, "0", "SP");
    symbols_insert(predefined_table, "1", "LCL");
    symbols_insert(predefined_table, "2", "ARG");
    symbols_insert(predefined_table, "3", "THIS");
    symbols_insert(predefined_table, "4", "THAT");
    symbols_insert(predefined_table, "0", "R0");
    symbols_insert(predefined_table, "1", "R1");
    symbols_insert(predefined_table, "2", "R2");
    symbols_insert(predefined_table, "3", "R3");
    symbols_insert(predefined_table, "4", "R4");
    symbols_insert(predefined_table, "5", "R5");
    symbols_insert(predefined_table, "6", "R6");
    symbols_insert(predefined_table, "7", "R7");
    symbols_insert(predefined_table, "8", "R8");
    symbols_insert(predefined_table, "9", "R9");
    symbols_insert(predefined_table, "10", "R10");
    symbols_insert(predefined_table, "11", "R11");
    symbols_insert(predefined_table, "12", "R12");
    symbols_insert(predefined_table, "13", "R13");
    symbols_insert(predefined_table, "14", "R14");
    symbols_insert(predefined_table, "15", "R15");
    symbols_insert(predefined_table, "16384", "SCREEN");
    symbols_insert(predefined_table, "24576", "KEYBOARD");
    symbols_insert(alu_table, "0101010", "0");
    symbols_insert(alu_table, "0111111", "1");
    symbols_insert(alu_table, "0111010", "-1");
    symbols_insert(alu_table, "0001100", "D");
    symbols_insert(alu_table, "0110000", "A");
    symbols_insert(alu_table, "0001101", "!D");
    symbols_insert(alu_table, "0110001", "!A");
    symbols_insert(alu_table, "0001111", "-D");
    symbols_insert(alu_table, "0110011", "-A");
    symbols_insert(alu_table, "0011111", "D+1");
    symbols_insert(alu_table, "0110111", "A+1");
    symbols_insert(alu_table, "0001110", "D-1");
    symbols_insert(alu_table, "0110010", "A-1");
    symbols_insert(alu_table, "0000010", "D+A");
    symbols_insert(alu_table, "0010011", "D-A");
    symbols_insert(alu_table, "0000111", "A-D");
    symbols_insert(alu_table, "0000000", "D&A");
    symbols_insert(alu_table, "0010101", "D|A");
    symbols_insert(alu_table, "1110000", "M");
    symbols_insert(alu_table, "1110001", "!M");
    symbols_insert(alu_table, "1110011", "-M");
    symbols_insert(alu_table, "1110111", "M+1");
    symbols_insert(alu_table, "1110010", "M-1");
    symbols_insert(alu_table, "1000010", "D+M");
    symbols_insert(alu_table, "1010011", "D-M");
    symbols_insert(alu_table, "1000111", "M-D");
    symbols_insert(alu_table, "1000000", "D&M");
    symbols_insert(alu_table, "1010101", "D|M");
    symbols_insert(dest_table, "000", "null");
    symbols_insert(dest_table, "001", "M");
    symbols_insert(dest_table, "010", "D");
    symbols_insert(dest_table, "011", "MD");
    symbols_insert(dest_table, "100", "A");
    symbols_insert(dest_table, "101", "AM");
    symbols_insert(dest_table, "110", "AD");
    symbols_insert(dest_table, "111", "AMD");
    symbols_insert(jump_table, "000", "null");
    symbols_insert(jump_table, "001", "JGT");
    symbols_insert(jump_table, "010", "JEQ");
    symbols_insert(jump_table, "011", "JGE");
    symbols_insert(jump_table, "100", "JLT");
    symbols_insert(jump_table, "101", "JNE");
    symbols_insert(jump_table, "110", "JLE");
    symbols_insert(jump_table, "111", "JMP");
}

// Hack Machine Code:
// program  ::= (instr tk_eol?)* tk_eoi
// instr    ::= tk_a_instr | tk_c_instr

// parse Hack Machine Code and return a vector of tokens, one per instruction
vector<Token> parse_program()
{
    vector<Token> instructions ;

    while ( have(tk_instruction) )
    {
        instructions.push_back(mustbe(tk_instruction)) ;
        if ( have(tk_eol) ) next_token() ;
    }
    mustbe(tk_eoi) ;

    return instructions ;
}

/*****************   REPLACE THE FOLLOWING CODE  ******************/

// disassemble a Hack Machine Code program without using any symbols
static void disassemble_no_symbols(vector<Token> instructions)
{   
    create_table();

    //for all instructions input, go through and find in the symbol table
    for ( int i=0; i<instructions.size(); i++ )
    {
        write_to_output("        ");

        //A instruction
        if ( token_kind(instructions[i]) == tk_a_instr )
        {   
            int value = token_ivalue(instructions[i]);
            write_to_output("@" + to_string(value) + '\n');
        }

        //C instruction
        if ( token_kind( instructions[i]) == tk_c_instr )
        {
            string dest = token_dest(instructions[i]);
            string alu = token_a_bit(instructions[i]) + token_alu_op(instructions[i]);
            string jump = token_jump(instructions[i]);

            //dest = comp ; jump
            //DEST:: check dest
            if ( dest == "000" )
            {
                write_to_output("");
            }
            else
            {
                write_to_output(symbols_lookup(dest_table, dest) + '=');
            }

            //COMP:: if not exits, output that error line
            if ( symbols_lookup(alu_table, alu) == "" )
            {
                write_to_output("< ** UNDEFINED ALU OPERATION ** >");
            }
            else
            {
                write_to_output(symbols_lookup(alu_table, alu));
            }

            //JUMP:: if jump bit is 000 ie no jump, then output empty string
            if( jump == "000" )
            {
                write_to_output("");
            }
            else
            {
                write_to_output(";" + symbols_lookup(jump_table, jump));
            }

            write_to_output("\n");
        }
    }
}

// convert Hack Machine Code into Assembly Language
// use predefined names, generic label names and generic variable names
static void disassemble_symbols_first(vector<Token> instructions)
{   
    create_table();

    // int label = 0;
    int current_label = 0;
    int variable_RAM_counter = 16;
    int variable_name_counter = 0;

    //first run to add some user defined label to symbol table
    for ( int i=0; i<instructions.size(); i++ )
    {
        //A instruction
        if ( token_kind(instructions[i]) == tk_a_instr )
        {      
            int value = token_ivalue(instructions[i]);
            if ( token_kind(instructions[i+1]) == tk_c_instr && token_jump(instructions[i+1]) != "000" )
            {   
                if ( value < instructions.size() )
                {   
                    label.push_back(value);
                }
            }
            else if ( token_a_bit(instructions[i+1]) == "1" || token_dest(instructions[i+1])[2] == '1' )
            {   
                if ( value == variable_RAM_counter )
                {   
                    string variable_name = "v_" + to_string(variable_name_counter);
                    symbols_insert(predefined_table, to_string(variable_RAM_counter), variable_name);
                    variable_RAM_counter++;
                    variable_name_counter++;
                }
            }
        }
    }

    for ( int i=0; i<label.size(); i++ )
    {
        for ( int j=0;j<label.size();j++ )
        {
            if ( label[i] < label[j] )
            {
                int temp = label[i];
                label[i] = label[j];
                label[j] = temp;
            }
        }
    }

    for ( int k=0; k<label.size(); k++ )
    {
        for ( int i=0; i<label.size(); i++ )
        {
            for ( int j=i+1;j<label.size();j++ )
            {
                if ( label[i] == label[j] )
                {
                    label.erase(label.begin()+j);
                }
            }
        } 
    }

    for ( int i=0; i<label.size(); i++ )
    {   
        string name = "L" + to_string(current_label);
        symbols_insert(symbol_table, to_string(label[i]), name);
        current_label++;
    }

    //second run to output hack language by mapping the code to symbol tables
    for ( int i=0; i<instructions.size(); i++ )
    {      
        if ( symbols_lookup(symbol_table, to_string(i)) != "" )
        {
            string search_symbol = "L" + to_string(i);
            write_to_output("(" + symbols_lookup(symbol_table, to_string(i)) + ")");
            write_to_output("\n");
        }

        //A instruction
        if ( token_kind(instructions[i]) == tk_a_instr )
        {      
            int value = token_ivalue(instructions[i]);
            if ( token_kind(instructions[i+1]) == tk_c_instr && token_jump(instructions[i+1]) != "000" )
            {   
                if ( value < instructions.size() )
                {   
                    write_to_output("        ");
                    write_to_output("@" + symbols_lookup(symbol_table, to_string(value)) + "\n");
                }
            }

            else if ( token_a_bit(instructions[i+1]) == "1" || token_dest(instructions[i+1])[2] == '1' )
            {  
                if ( symbols_lookup(predefined_table, to_string(value)) != "" )
                {   
                    write_to_output("        ");
                    write_to_output("@"  + symbols_lookup(predefined_table, to_string(value)) + "\n");
                }
            }
            
            else
            {   
                write_to_output("        ");
                write_to_output("@" + to_string(value) + "\n");
            }

        }
        //C instruction
        if ( token_kind( instructions[i]) == tk_c_instr )
        {   
            write_to_output("        ");
            string dest = token_dest(instructions[i]);
            string alu = token_a_bit(instructions[i]) + token_alu_op(instructions[i]);
            string jump = token_jump(instructions[i]);

        //     //dest = comp ; jump
        //     //DEST:: check dest
            if (dest == "000")
            {
                write_to_output("");
            }
            else
            {
                write_to_output(symbols_lookup(dest_table, dest) + '=');
            }

            //COMP:: if not exits, output that error line
            if ( symbols_lookup(alu_table, alu) == "" )
            {
                write_to_output("< ** UNDEFINED ALU OPERATION ** >");
            }
            else
            {
                write_to_output(symbols_lookup(alu_table, alu));
            }

            //JUMP:: if jump bit is 000 ie no jump, then output empty string
            if ( jump == "000" )
            {
                write_to_output("");
            }
            else
            {
                write_to_output(";" + symbols_lookup(jump_table, jump));
            }
            write_to_output("\n");
        }
    }
}

/*****************        DOWN TO HERE         ******************/

// main program
int main(int argc,char **argv)
{   
    
    // initialise tokeniser
    next_token() ;

    // is the first command line argument "N", ie Numeric Output ?
    if ( argc > 1 && string(argv[1]) == "N" )
    {
        // YES, output all A-instructions in numeric form
        disassemble_no_symbols(parse_program()) ;
    }
    else
    {
        // NO, output A-instructions in symbolic form where possible
        disassemble_symbols_first(parse_program()) ;
    }

    // flush the output and any errors
    print_output() ;
    print_errors() ;
}

