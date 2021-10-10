// these functions are responsble for remembering the input characters as they
// are read by read_next_char(). The characters are passed in as unicode
// code points and must be encoded as UTF-8
//

#include "iobuffer.h"
#include <iostream>
#include <vector>


// to shorten the code
using namespace std ;
using namespace CS_IO_Buffers ;

// UTF-8 Character Encoding
// How to convert a legal unicode code-point U into characters c0c1c2c3
// . count how many bits are required after removing leading 0s?
// . if <= 7,  c0 = U & 0x7F
// . if <= 11, c0 = 0xC0 | ((U >>  6) & 0x1F), c1 = 0x80 | (U & 0x3F)
// . if <= 16, c0 = 0xE0 | ((U >> 12) & 0x0F), c1 = 0x80 | ((U >>  6) & 0x3F), c2 = 0x80 | (U & 0x3F)
// . if <= 21, c0 = 0xF0 | ((U >> 18) & 0x07), c1 = 0x80 | ((U >> 12) & 0x3F), c2 = 0x80 | ((U >> 6) & 0x3F), c3 = 0x80 | (U & 0x3F) 
//
// Notes:
// . you should assume that the remember() function is only passed legal unicode code-points
// . C++ strings just store sequences of single byte characters and UTF-8 is just a sequence of bytes
// . the column counting is performed in bytes so you do not need to know how many UTF-8 characters the bytes represent
//
namespace Assignment_Tokeniser
{
    // the line number of the next character to be remembered, starts at 1
    static int line = 1 ;
    int get_line()
    {
        return line ;
    }

    // the column number of the next character to be remembered, starts at 1
    // column numbers are in bytes not characters
    static int column = 1 ;
    int get_column()
    {
        return column ;
    }

    // temporary memory solution so we can build ASCII tokens
    static string memory = "" ;

    // remember character ch
    // ch is a legal unicode code-point in the range 0 to 0x10FFFF
    void remember(int ch)
    {
        // initially assume all input is ASCII - only guaranteed true for Milestone Tests
        memory += (char)(ch & 0x7F) ;
    }

    // return a remembered line with trailing newline character
    // the last line may or may not have a trailing newline character
    // returns "" if ln is less than 1 or ln is greater than the last remembered line
    // the string is encoded in UTF-8
    string remembered_line(int ln)
    {
        return "" ;
    }

    // returns all characters remembered since the last call
    // the string is encoded in UTF-8
    string last_characters()
    {
        string last = memory ;
        memory = "" ;
        return last ;
    }
}
