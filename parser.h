#ifndef PARSER_H
#define PARSER_H
#include "token.h"
#include "node.h"

extern std::ifstream in_file;

Node* parser();
Node* program();
Node* block(unsigned int depth);
Node* vars(unsigned int depth);
Node* expr(unsigned int depth);
Node* n(unsigned int depth);
Node* a(unsigned int depth);
Node* m(unsigned int depth);
Node* r(unsigned int depth);
Node* stats(unsigned int depth);
Node* mstat(unsigned int depth);
Node* stat(unsigned int depth);
Node* in(unsigned int depth);
Node* out(unsigned int depth);
Node* iff(unsigned int depth);
Node* loop(unsigned int depth);
Node* assign(unsigned int depth);
Node* ro(unsigned int depth);
Node* gotoo(unsigned int depth);
Node* label(unsigned int depth);

void error1(tokens need, Token got);
void error2(tokens need_1, tokens need_2, Token got);
void error3(tokens need_1, tokens need_2, tokens need_3, Token got);
void error_mult(Token got);

void print_tree(Node* node);

#endif