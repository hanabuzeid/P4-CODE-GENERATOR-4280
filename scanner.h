#ifndef SCANNER_H
#define SCANNER_H
#include "token.h"
#include <string>
#include <map>
#include <fstream>

Token scanner(std::ifstream& in_file, unsigned int& lineNum);
int setFSAcol(char current_char);
Token setToken(int state, std::string word, unsigned int lineNum);

#endif