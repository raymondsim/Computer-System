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
}

// convert Hack Machine Code into Assembly Language
// use predefined names, generic label names and generic variable names
static void disassemble_symbols(vector<Token> instructions)
{
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
        disassemble_symbols(parse_program()) ;
    }

    // flush the output and any errors
    print_output() ;
    print_errors() ;
}

