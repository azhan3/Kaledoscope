#include <iostream>

using namespace std;

enum Token {
    tok_eof = -1,

    tok_def = -2,
    tok_extern = -3,

    tok_identifier = -4,
    tok_number = -5,
};

static string IdentifierStr;
static double NumVal;

static int gettok() {
    static int LastChar = ' ';

    while (isspace(LastChar)) { 
        /*
            std::isspace reads 0x20 ' ', 0x0c '\f', 0x0a '\n', 0x0d '\r', 0x09 '\t', 0x0b '\v' as whitespace
        */
        LastChar = getchar(); // getchar() gets the current character from standard input
    }

    if (isalpha(LastChar)) { // variable names must start with alphabetical character
        IdentifierStr = LastChar;

        while (isalnum((LastChar = getchar()))) { // isalnum checks if character is alphanumeric, in other words whether c is either a decimal digit or a uppercase / lowercase letter
            IdentifierStr += LastChar;
        } 

        if (IdentifierStr == "def") {
            return tok_def;
        }
        if (IdentifierStr == "extern") {
            return tok_extern;
        }
        return tok_identifier;
    }
    /*
        This is straightforward code for process numeric input. Note: that this isn't doing sufficient error checking: it will incorrectly read "1.23.45.67" and handle it as if you typed in "1.23" due to the char ** endptr in strtod being set to 0
    */
    if (isdigit(LastChar) || LastChar == '.') { 
        string NumStr;

        do {
            NumStr += LastChar;
            LastChar = getchar();
        } while (isdigit(LastChar) || LastChar == '.');

        NumVal = strtod(NumStr.c_str(), 0); 
        /*
            double strtod(const char* str, char** endptr);

            std::strod converts a C-style string (const char*) to a double
            
            NumStr.c_str() converts the C++ std::string object Numstr into a C-style string (const char*), which std::strod requires

            0 (or nullptr) is the second parameter for std::strod, which is an optional "end pointer" (char**). Passing 0 (or nullptr) tells std::strod to ignore this output, which would otherwise point to the character after the last character converted in the string
        */
        return tok_number;
    }
    /*
        Handle comments by skipping to the end of the line and then return the next token. Finally, if the input doesn't match one of the above cases, it is either an operator character like '+' or the end of the file
    */
    if (LastChar == '#') {
        // comment until end of line
        do {
            LastChar = getchar();
        } while (LastChar != EOF && LastChar != '\n' && LastChar != '\r');

        if (LastChar != EOF) {
            return gettok();
        }
    }
    /*
        Check for end of file. DO NOT LET THE LEXER EAT THE EOF
    */
    if (LastChar == EOF) {
        return tok_eof;
    }
    /*
        Otherwise, just return the character as its ascii value
    */
    int ThisChar = LastChar;
    LastChar = getchar();
    return ThisChar;
}

int main() {
    cout << gettok() << endl;
    return 0;
}