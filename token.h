#ifndef TOKEN_H
#define TOKEN_H

#include <string>

enum tokens {
    BEGIN_TK, END_TK,  LOOP_TK, WHILE_TK, VOID_TK,  EXIT_TK, GETTER_TK, OUTTER_TK,                
    MAIN_TK, IF_TK, THEN_TK,  ASSIGN_TK, DATA_TK, PROC_TK, EQUALS_TK,                
    EQUALS_OR_LESS_THAN_TK, EQUALS_OR_GREAT_THAN_TK, EQUALS_EQUALS_TK,  COLON_TK,                 
    COLON_EQUALS_TK, PLUS_TK, MINUS_TK, ASTERISK_TK, SLASH_TK, PERCENT_TK,               
    PERIOD_TK, LEFT_PAREN_TK, RIGHT_PAREN_TK, COMMA_TK, LEFT_BRACE_TK,            
    RIGHT_BRACE_TK, SEMI_COLON_TK, LEFT_BRACKET_TK, RIGHT_BRACKET_TK, ID_TK,                    
    INT_TK, EOF_TK, ERROR_TK       
};

struct Token {
    unsigned int lineNum;      
    tokens tokenId;           
    std::string tokenString; 

    Token(){
        this->tokenId = ERROR_TK;
        this->tokenString = "";
        this->lineNum = 0;
    }
    
    Token(tokens ID, std::string string, unsigned int lineNum)
    {
        this->lineNum = lineNum;
        this->tokenId = ID;
        this->tokenString = string;
    }
};

#endif