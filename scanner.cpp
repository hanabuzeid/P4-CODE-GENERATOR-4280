#include "scanner.h"
#include "token.h"
#include <string>
#include <map>
#include <fstream>
#include <iostream>

int fsa_table[23][23] = { 
    {  0,   1,  23,   2,   3,  23,  23,   7,   9,  10,  11,  12,  13,  14,  15,  16,  17,  18,  19,  20,  21,  22,  -1}, 
    {100,   1,   1, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100}, //id
    {101, 101, 101,   2, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101}, // int
    {102, 102, 102, 102,   6,   5,   4, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102}, // =
    {103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103}, //=>
    {104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104}, //=<
    {105, 105, 105, 105, 105, 105, 105, 105, 105, 105, 105, 105, 105, 105, 105, 105, 105, 105, 105, 105, 105, 105, 105}, // ==
    {106, 106, 106, 106,   8, 106, 106, 106, 106, 106, 106, 106, 106, 106, 106, 106, 106, 106, 106, 106, 106, 106, 106}, // :
    {107, 107, 107, 107, 107, 107, 107, 107, 107, 107, 107, 107, 107, 107, 107, 107, 107, 107, 107, 107, 107, 107, 107}, // :=
    {108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108}, // +
    {109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109}, // -
    {110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110, 110}, //*
    {111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111}, // /
    {112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112}, // %
    {113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113}, // .
    {114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114}, // (
    {115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115}, // )
    {116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116}, // ,
    {117, 117, 117, 117, 117, 117, 117, 117, 117, 117, 117, 117, 117, 117, 117, 117, 117, 117, 117, 117, 117, 117, 117}, //  {
    {118, 118, 118, 118, 118, 118, 118, 118, 118, 118, 118, 118, 118, 118, 118, 118, 118, 118, 118, 118, 118, 118, 118}, // }
    {119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119}, //  ;
    {120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120}, // [
    {121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121, 121}  // ]
};

std::map<std::string, tokens> keywords = {
    {"begin", BEGIN_TK}, {"end", END_TK}, {"loop", LOOP_TK},
    {"while", WHILE_TK}, {"void", VOID_TK}, {"exit", EXIT_TK},
    {"getter", GETTER_TK},{"outter", OUTTER_TK},{"main", MAIN_TK},
    {"if", IF_TK},{"then", THEN_TK},{"assign", ASSIGN_TK},
    {"data", DATA_TK},{"proc", PROC_TK}
};

std::map<char, int> symbols = {
    {'=', 4},{'<', 5},{'>', 6},
    {':', 7},{'+', 8},{'-', 9},
    {'*', 10},{'/', 11}, {'%', 12}, {'.', 13},
    {'(', 14}, {')', 15},{',', 16},
    {'{', 17},{'}', 18},{';', 19},
    {'[', 20},{']', 21}
};

std::map<int, tokens> endState = {
    {100, ID_TK}, {101, INT_TK},  {-1, EOF_TK},
    {102, EQUALS_TK}, {103, EQUALS_OR_GREAT_THAN_TK},
    {104, EQUALS_OR_LESS_THAN_TK}, {105, EQUALS_EQUALS_TK},
    {106, COLON_TK}, {107, COLON_EQUALS_TK}, {108, PLUS_TK},
    {109, MINUS_TK},{110, ASTERISK_TK},{111, SLASH_TK},
    {112, PERCENT_TK}, {113, PERIOD_TK},{114, LEFT_PAREN_TK},
    {115, RIGHT_PAREN_TK},{116, COMMA_TK}, {117, LEFT_BRACE_TK},
    {118, RIGHT_BRACE_TK},{119, SEMI_COLON_TK},
    {120, LEFT_BRACKET_TK},{121, RIGHT_BRACKET_TK}
};


Token scanner(std::ifstream& in_file, unsigned int& lineNum) {
    int state = 0;                
    int lookAhead = 0;       
    char currentChar = ' ';      
    std::string word = "";   

    while (state < 100) {      
        in_file.get(currentChar);

        if (currentChar == '$') {
            in_file.get(currentChar);
            if (currentChar == '$') {            
                in_file.get(currentChar);
                while (1) {                     
                    in_file.get(currentChar);
                    if (in_file.eof()) {
                        std::cout << "SCANNER ERROR: Comment didnt end! EOF reached line: " << lineNum << std::endl;  
                        return Token(ERROR_TK, "COMMENT NOT CLOSED", lineNum);
                    }
                    if (currentChar == '$') {
                        in_file.get(currentChar);
                        if (currentChar == '$') {
                            in_file.get(currentChar); 
                            break;
                        }
                    }
                }
            }
        }

        int colFSA = setFSAcol(currentChar);   

        if (in_file.eof()) {  
            colFSA = 22;
        }

        if (colFSA == 23)  
        {

            std::cout << "SCANNER ERROR: INVALID CHARACTER \'" << currentChar << "\'";
            std::cout << " AT LINE: " << lineNum << std::endl;
            return Token(ERROR_TK, "INVALID CHAR", lineNum);
        }

        lookAhead = fsa_table[state][colFSA];

        if (lookAhead == 23) {  
            std::cout << "SCANNER ERROR 2: INVALID CHAR \"" << currentChar << "\"";//.
            std::cout << " AT LINE: " << lineNum << std::endl;
            return Token(ERROR_TK, "INVALID ID", lineNum);
        }

        else if (lookAhead == -1) {
            return Token(EOF_TK, "EOF", lineNum);
        }

        else if (lookAhead >= 100)
        {
            in_file.unget();
            return setToken(lookAhead, word, lineNum);
        }
        else
        {

            if (!isspace(currentChar))
            {
                word += currentChar;
            }

            if (word.length() >= 9) 
            {
                std::cout << "SCANNER ERROR 3: \"" << word << "\" longer than 8 chars";
                std::cout << " AT LINE: " << lineNum << std::endl;
                return Token(ERROR_TK, "INVALID LENGTH: MAKE IT SHORTER", lineNum);
            }
            if (currentChar == '\n')
            {
                lineNum++;
            }

            state = lookAhead;
        }
    }

    return Token(ERROR_TK, "SCANNER FAILED... EXITING", lineNum);

}

int setFSAcol(char currentChar) {

    if (currentChar == EOF)
        return 22;

    if (isspace(currentChar))
        return 0;  

    if (isalpha(currentChar) || currentChar == '_') {
        if (isupper(currentChar))
            return 2; 
        return 1; 
    }

    if (isdigit(currentChar))
        return 3; 


    else 
    {
        if (symbols.find(currentChar) != symbols.end())
            return symbols[currentChar];
    }

    return 23; 
}

Token setToken(int state, std::string word, unsigned int lineNum) {
   
    if (keywords.find(word) != keywords.end()) {
        return Token(keywords[word], word, lineNum);
    }
    else {
        return Token(endState[state], word, lineNum);
    }
}

