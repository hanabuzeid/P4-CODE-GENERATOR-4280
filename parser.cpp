#include "parser.h"
#include "scanner.h"
#include <iostream>

std::ifstream in_file;
unsigned int lineNum = 1;
Token token;

std::string tokes[] = {"BEGIN_TK", "END_TK", "LOOP_TK", "WHILE_TK", "VOID_TK", "EXIT_TK", 
"GETTER_TK", "OUTTER_TK", "MAIN_TK", "IF_TK", "THEN_TK", "ASSIGN_TK", "DATA_TK", "PROC_TK",
"EQUALS_TK", "EQUALS_OR_LESS_THAN_TK", "EQUALS_OR_GREAT_THAN_TK", "EQUALS_EQUALS_TK", "COLON_TK",
"COLON_EQUALS_TK", "PLUS_TK", "MINUS_TK", "ASTERISK_TK", "SLASH_TK", "PERCENT_TK", "PERIOD_TK", 
"LEFT_PAREN_TK", "RIGHT_PAREN_TK", "COMMA_TK", "LEFT_BRACE_TK", "RIGHT_BRACE_TK",
"SEMI_COLON_TK", "LEFT_BRACKET_TK", "RIGHT_BRACKET_TK", "ID_TK", "INT_TK", "EOF_TK", "ERROR_TK"};

bool deBug = false;
bool deBug1 = false;

Node *parser()
{
    if (deBug)
    {
        std::cout << "parser" << "\n";
    }

    Node *node;
    token = scanner(in_file, lineNum);
    if (deBug1)
    {
        std::cout << "Working on TOKEN 1: " << token.tokenString << "\n";
    }

    node = program();
    if (token.tokenId == EOF_TK)
    {
        std::cout << "PARSING FINISHED!" << std::endl;
        return node;
    }
    else
        error1(EOF_TK, token);
}

Node *program()
{
    if (deBug)
    {
        std::cout << "program"<< "\n";
    } 
    int depth = 0;

    Node *node = new Node("<program>", depth);

    node->child1 = vars(depth);  
    if (token.tokenId == MAIN_TK) 
    {
        node->token1 = token;     
        token = scanner(in_file, lineNum); 
        if (deBug1)
        {
            std::cout << "Working on TOKEN 2: " << token.tokenString << "\n";
        }
        node->child2 = block(depth);
    }
    else
        error1(MAIN_TK, token);
    return node;
}

Node *block(unsigned int depth)
{
    if (deBug)
    {
        std::cout << "block" << "\n";
    }
    depth++;
    Node *node = new Node("<block>", depth);
    if (token.tokenId == BEGIN_TK)
    {
        node->token1 = token;
        token = scanner(in_file, lineNum);
        if (deBug1)
        {
            std::cout << "Working on TOKEN 3: " << token.tokenString << "\n";
        }
        node->child1 = vars(depth);
        node->child2 = stats(depth);

        if (token.tokenId == END_TK)
        {
            node->token2 = token;
            token = scanner(in_file, lineNum);
            if (deBug1)
            {
                std::cout << "Working on TOKEN 4: " << token.tokenString << "\n";
            }

            return node;
        }
        else
            error1(END_TK, token);
    }
    else
        error1(BEGIN_TK, token);
}

Node *vars(unsigned int depth)
{
    if (deBug)
    {
        std::cout << "vars" << "\n";
    }
    depth++;
    Node *node = new Node("<vars>", depth);

    if (token.tokenId == DATA_TK)
    {
        node->token1 = token;
        token = scanner(in_file, lineNum);
        if (deBug1)
        {
            std::cout << "Working on TOKEN 5: " << token.tokenString << "\n";
        }
        if (token.tokenId == ID_TK)
        {
            node->token2 = token;
            token = scanner(in_file, lineNum);
            if (deBug1)
            {
                std::cout << "Working on TOKEN 6: " << token.tokenString << "\n";
            } 
            if (token.tokenId == COLON_EQUALS_TK)
            {
                node->token3 = token;
                token = scanner(in_file, lineNum);
                if (deBug1)
                {
                    std::cout << "Working on TOKEN 7: " << token.tokenString << "\n";
                } 
                if (token.tokenId == INT_TK)
                {
                    node->token4 = token;
                    token = scanner(in_file, lineNum);
                    if (deBug1)
                    {
                        std::cout << "Working on TOKEN 8: " << token.tokenString << "\n";
                    } 
                    if (token.tokenId == SEMI_COLON_TK)
                    {
                        node->token5 = token;
                        token = scanner(in_file, lineNum);
                        if (deBug1)
                        {
                            std::cout << "Working on TOKEN 9: " << token.tokenString << "\n";
                        }
                        node->child1 = vars(depth);
                        return node;
                    }
                    else
                    {
                        if (deBug)
                        {
                            std::cout << "yeP" << "\n";
                        }
                        error1(SEMI_COLON_TK, token);
                    }
                }
                else
                    error1(INT_TK, token);
            }
            else
                error1(COLON_EQUALS_TK, token);
        }
        else
            error1(ID_TK, token);
    }
    else
        return NULL;
}


Node *expr(unsigned int depth)// <expr>  ->  <N> - <expr>  | <N>
{
    if (deBug)
    {
        std::cout << "expr" << "\n";
    }
    depth++;
    Node *node = new Node("<expr>", depth);

    node->child1 = n(depth);
    if (token.tokenId == MINUS_TK)
    {
        node->token1 = token;
        token = scanner(in_file, lineNum);
        if (deBug1)
        {
            std::cout << "Working on TOKEN 11: " << token.tokenString << "\n";
        } 
        node->child2 = expr(depth);
        return node;
    }
    else
        return node;
}


Node *n(unsigned int depth) // <N>  ->   <A> / <N> | <A> * <N> | <A>
{
    if (deBug)
    {
        std::cout << "n" << "\n";
    }
    depth++;
    Node *node = new Node("<N>", depth);
    node->child1 = a(depth);
    if (token.tokenId == SLASH_TK)
    {
        node->token1 = token;
        token = scanner(in_file, lineNum);
        if (deBug1)
        {
            std::cout << "Working on TOKEN 12: " << token.tokenString << "\n";
        } 
        node->child2 = n(depth);
        return node;
    }
    else if (token.tokenId == ASTERISK_TK)
    {
        node->token1 = token;
        token = scanner(in_file, lineNum);
        if (deBug1)
        {
            std::cout << "Working on TOKEN 13: " << token.tokenString << "\n";
        }
        node->child2 = n(depth);
        return node;
    }
    else
        return node;
}

Node *a(unsigned int depth)
{
    if (deBug)
    {
        std::cout << "a" << "\n";
    } 
    depth++;
    Node *node = new Node("<A>", depth);
    node->child1 = m(depth);
    if (token.tokenId == PLUS_TK)
    {
        node->token1 = token;
        token = scanner(in_file, lineNum);
        if (deBug1)
        {
            std::cout << "Working on TOKEN 14: " << token.tokenString << "\n";
        } 
        node->child2 = a(depth);
        return node;
    }
    else
    { 
        return node;
    } 
}

Node *m(unsigned int depth)
{
    if (deBug)
    {
        std::cout << "m" << "\n";
    }
    depth++;
    Node *node = new Node("<M>", depth);
    if (token.tokenId == ASTERISK_TK)
    {
        node->token1 = token;
        token = scanner(in_file, lineNum);
        if (deBug1)
        {
            std::cout << "Working on TOKEN 15: " << token.tokenString << "\n";
        } 
        node->child1 = m(depth);
        return node;
    }
    else
    {
        node->child1 = r(depth);
        return node;
    }
}

Node *r(unsigned int depth)
{
    if (deBug)
    {
        std::cout << "r" << "\n";
    } 
    depth++;
    Node *node = new Node("<R>", depth);
    if (token.tokenId == LEFT_PAREN_TK)
    {
        node->token1 = token;
        token = scanner(in_file, lineNum);
        if (deBug1)
        {
            std::cout << "Working on TOKEN 16: " << token.tokenString << "\n";
        } 
        node->child1 = expr(depth);

        if (token.tokenId == RIGHT_PAREN_TK)
        {
            node->token2 = token;

            token = scanner(in_file, lineNum);
            if (deBug1)
            {
                std::cout << "Working on TOKEN 17: " << token.tokenString << "\n";
            } 
            return node;
        }
        else
            error1(RIGHT_PAREN_TK, token);
    }
    else if (token.tokenId == ID_TK)
    {
        node->token1 = token;
        token = scanner(in_file, lineNum);
        if (deBug1)
        {
            std::cout << "Working on TOKEN 18: " << token.tokenString << "\n";
        } 
        return node;
    }
    else if (token.tokenId == INT_TK)
    {
        node->token1 = token;
        token = scanner(in_file, lineNum);
        if (deBug1)
        {
            std::cout << "Working on TOKEN 19: " << token.tokenString << "\n";
        } 
        return node;
    }
    else
        error3(LEFT_PAREN_TK, ID_TK, INT_TK, token);
}

Node *mstat(unsigned int depth)
{
    if (deBug)
    {
        std::cout << "mstat" << "\n";
    }
    depth++;
    Node *node = new Node("<mstat>", depth);

    if (token.tokenId == GETTER_TK || token.tokenId == OUTTER_TK || token.tokenId == BEGIN_TK || token.tokenId == IF_TK || token.tokenId == LOOP_TK || token.tokenId == PROC_TK || token.tokenId == ASSIGN_TK || token.tokenId == VOID_TK)
    {
        if (deBug1)
        {
            std::cout << "CURRENTLY WORKING ..." << token.tokenString << "\n";
        }
        node->child1 = stat(depth);
        node->child2 = mstat(depth);
        return node;
    }
    else
        return node;
}

Node *in(unsigned int depth)
{
    if (deBug)
    {
        std::cout << "in"<< "\n";
    }
    depth++;
    Node *node = new Node("<in>", depth);
    if (token.tokenId == GETTER_TK)
    {
        token = scanner(in_file, lineNum);
        if (deBug1)
        {
            std::cout << "Working on TOKEN 36: " << token.tokenString << "\n";
        }
        if (token.tokenId == ID_TK)
        {
            node->token1 = token;
            token = scanner(in_file, lineNum); 
            return node;
        }
        else
            error1(ID_TK, token);
    }
    else
        error1(GETTER_TK, token);
}

Node *stat(unsigned int depth)
{
    if (deBug)
    {
        std::cout << "stat"<< "\n";
    } 
    depth++;
    Node *node = new Node("<stat>", depth);

    if (token.tokenId == GETTER_TK)
    {
        node->child1 = in(depth);
        if (token.tokenId == SEMI_COLON_TK)
        {
            node->token1 = token;
            token = scanner(in_file, lineNum);
            if (deBug1)
            {
                std::cout << "Working on TOKEN 22: " << token.tokenString << "\n";
            }
            return node;
        }
        else
            error1(SEMI_COLON_TK, token);
    }
    else if (token.tokenId == OUTTER_TK)
    {
        if (deBug)
        {
            std::cout << "out= tru" << "\n";
        } 
        node->child1 = out(depth);
        if (token.tokenId == SEMI_COLON_TK)
        {
            node->token1 = token;
            token = scanner(in_file, lineNum);
            if (deBug1)
            {
                std::cout << "Working on TOKEN 24: " << token.tokenString << "\n";
            }
            return node;
        }
        else
            error1(SEMI_COLON_TK, token);
    }
    else if (token.tokenId == BEGIN_TK)
    {
        node->child1 = block(depth);
        return node;
    }
    else if (token.tokenId == IF_TK)
    {
        node->child1 = iff(depth);
        if (token.tokenId == SEMI_COLON_TK)
        {
            node->token1 = token;
            token = scanner(in_file, lineNum);
            if (deBug1)
            {
                std::cout << "Working on TOKEN 28: " << token.tokenString << "\n";
            }
            return node;
        }
        else
            error1(SEMI_COLON_TK, token);
    }
    else if (token.tokenId == LOOP_TK)
    {
        node->child1 = loop(depth);
       if (token.tokenId == SEMI_COLON_TK)
        {
            node->token1 = token;
            token = scanner(in_file, lineNum);
            if (deBug1)
            {
                std::cout << "Working on TOKEN 31: " << token.tokenString << "\n";
            }
            return node;
        }
        else
            error1(SEMI_COLON_TK, token);
    }
    else if (token.tokenId == ASSIGN_TK)
    {
        node->child1 = assign(depth);
        if (token.tokenId == SEMI_COLON_TK)
        {
            node->token1 = token;
            token = scanner(in_file, lineNum);
            if (deBug1)
            {
                std::cout << "Working on TOKEN 35: " << token.tokenString << "\n";
            }
            return node;
        }
        else
            error1(SEMI_COLON_TK, token);
    }
    else if (token.tokenId == PROC_TK)//.
    {
        node->child1 = gotoo(depth);
         if (token.tokenId == SEMI_COLON_TK)
        {
            node->token1 = token;
            token = scanner(in_file, lineNum);
            if (deBug1)
            {
                std::cout << "Working on TOKEN 33: " << token.tokenString << "\n";
            }
            return node;
        }
        else
            error1(SEMI_COLON_TK, token);
    }

    else
        error_mult(token);
}

Node *out(unsigned int depth)
{
    if (deBug)
    {
        std::cout << "out"<< "\n";
    }
    depth++;
    Node *node = new Node("<out>", depth);
    if (token.tokenId == OUTTER_TK)
    {

        token = scanner(in_file, lineNum);
        if (deBug1)
        {
            std::cout << "Working on TOKEN 37: " << token.tokenString << "\n";
        } 
        node->child1 = expr(depth);
        return node;
    }
    else
        error1(OUTTER_TK, token);
}

Node *iff(unsigned int depth)
{
    if (deBug)
    {
        std::cout << "if"<< "\n";
    }
    depth++;
    Node *node = new Node("<if>", depth);
    if (token.tokenId == IF_TK)
    {
        node->token1 = token;
        token = scanner(in_file, lineNum);
        if (deBug1)
        {
            std::cout << "Working on TOKEN 38: " << token.tokenString << "\n";
        }
        if (token.tokenId == LEFT_BRACKET_TK)
        {
            node->token2 = token;
            token = scanner(in_file, lineNum);
            if (deBug1)
            {
                std::cout << "Working on TOKEN 39: " << token.tokenString << "\n";
            }
            node->child1 = expr(depth);
            node->child2 = ro(depth);
            node->child3 = expr(depth);
            if (token.tokenId == RIGHT_BRACKET_TK)
            {
                node->token3 = token;
                if (token.tokenId == THEN_TK)
                {
                    node->token4 = token;
                    token = scanner(in_file, lineNum);
                    if (deBug1)
                    {
                        std::cout << "Working on TOKEN 40: " << token.tokenString << "\n";
                    }
                    node->child_4 = stat(depth);
                    return node;
                }
                else
                    error1(THEN_TK, token);
            }
            else
                error1(RIGHT_BRACKET_TK, token);
        }
        else
            error1(LEFT_BRACKET_TK, token);
    }
    else
        error1(LOOP_TK, token);
}

Node *assign(unsigned int depth)
{
    if (deBug)
    {
        std::cout << "assign"<< "\n";
    }
    depth++;
    Node *node = new Node("<assign>", depth);
    if (token.tokenId == ASSIGN_TK)
    {
        node->token1 = token;
        token = scanner(in_file, lineNum);
        if (deBug1)
        {
            std::cout << "Working on TOKEN 49: " << token.tokenString << "\n";
        }
        if (token.tokenId == ID_TK)
        {
            node->token2 = token;
            token = scanner(in_file, lineNum);
            if (deBug1)
            {
                std::cout << "Working on TOKEN 41: " << token.tokenString << "\n";
            }
            if (token.tokenId == COLON_EQUALS_TK)
            {
                token = scanner(in_file, lineNum);
                if (deBug1)
                {
                    std::cout << "Working on TOKEN 42: " << token.tokenString << "\n";
                }
                node->child1 = expr(depth);
                return node;
            }
            else
                error1(COLON_EQUALS_TK, token);
        }
        else
            error1(ID_TK, token);
    }
    else
        error1(ASSIGN_TK, token);
}

void print_tree(Node *node)
{
    if (node == nullptr)
    {
        std::cout << "EMPTY NODE" << std::endl;
        return;
    }
    else
    {
        std::string indents((node->depth * 2), ' ');
        std::cout << indents;
        std::cout << node->name << "  ";
        std::cout << node->token1.tokenString << " " << node->token2.tokenString << " " << node->token3.tokenString << " " << node->token4.tokenString << " " << node->token5.tokenString << " " << node->token6.tokenString << std::endl;

        if (node->child1 != nullptr)
            print_tree(node->child1);
        if (node->child2 != nullptr)
            print_tree(node->child2);
        if (node->child3 != nullptr)
            print_tree(node->child3);
        if (node->child_4 != nullptr)
            print_tree(node->child_4);
    }
    return;
}

Node *stats(unsigned int depth)
{
    if (deBug)
    {
        std::cout << "stats" << "\n";
    }
    depth++;
    Node *node = new Node("<stats>", depth);
    node->child1 = stat(depth);
    node->child2 = mstat(depth);
    return node;
}

Node *ro(unsigned int depth)
{
    if (deBug)
    {
        std::cout << "ro"<< "\n";
    }
    depth++;
    Node *node = new Node("<RO>", depth);

    if (token.tokenId == EQUALS_OR_LESS_THAN_TK)
    {
        node->token1 = token;
        token = scanner(in_file, lineNum);
        if (deBug1)
        {
            std::cout << "Working on TOKEN 49: " << token.tokenString << "\n";
        }
        return node;
    }

    else if (token.tokenId == EQUALS_OR_GREAT_THAN_TK)
    {
        node->token1 = token;
        token = scanner(in_file, lineNum);
        if (deBug1)
        {
            std::cout << "Working on TOKEN 49: " << token.tokenString << "\n";
        }
        return node;
    }
    else if (token.tokenId == EQUALS_EQUALS_TK)
    {
        node->token1 = token;
        token = scanner(in_file, lineNum);
        if (deBug1)
        {
            std::cout << "Working on TOKEN 49: " << token.tokenString << "\n";
        }
        return node;
    }
    else if (token.tokenId == PERCENT_TK)
    {
        node->token1 = token;
        token = scanner(in_file, lineNum);
        if (deBug1)
        {
            std::cout << "Working on TOKEN 49: " << token.tokenString << "\n";
        }
        return node;
    }
    else if (token.tokenId == LEFT_BRACKET_TK)
    {
        node->token1 = token;
        token = scanner(in_file, lineNum);
        if (deBug1)
        {
            std::cout << "Working on TOKEN 49: " << token.tokenString << "\n";
        }
        if (token.tokenId == EQUALS_EQUALS_TK)
        {
            node->token2 = token;
            token = scanner(in_file, lineNum);
            if (deBug1)
            {
                std::cout << "Working on TOKEN 41: " << token.tokenString << "\n";
            }
            if (token.tokenId == RIGHT_BRACKET_TK)
            {
                node->token3 = token;
                token = scanner(in_file, lineNum);
                if (deBug1)
                {
                    std::cout << "Working on TOKEN 61: " << token.tokenString << "\n";
                }
                node->token4 = token;
                token = scanner(in_file, lineNum);
                if (deBug1)
                {
                    std::cout << "Working on TOKEN 62: " << token.tokenString << "\n";
                }
                node->token5 = token;
                token = scanner(in_file, lineNum);
                if (deBug1)
                {
                    std::cout << "Working on TOKEN 63: " << token.tokenString << "\n";
                }
                node->token6 = token;
                return node;
            }
            else
                error1(RIGHT_BRACKET_TK, token);
        }
        else
            error1(EQUALS_EQUALS_TK, token);
    }

    else
        error_mult(token);
}

Node *loop(unsigned int depth)
{
    if (deBug)
    {
        std::cout << "loop" << "\n";
    }
    depth++;
    Node *node = new Node("<loop>", depth);
    if (token.tokenId == LOOP_TK)
    {
        node->token1 = token;
        token = scanner(in_file, lineNum);
        if (deBug1)
        {
            std::cout << "Working on TOKEN 38: " << token.tokenString << "\n";
        }
        if (token.tokenId == LEFT_BRACKET_TK)
        {
            node->token2 = token;
            token = scanner(in_file, lineNum);
            if (deBug1)
            {
                std::cout << "Working on TOKEN 39: " << token.tokenString << "\n";
            }
            node->child1 = expr(depth);
            node->child2 = ro(depth);
            node->child3 = expr(depth);
            if (token.tokenId == RIGHT_BRACKET_TK)
            {
                node->token3 = token;
                token = scanner(in_file, lineNum);
                if (deBug1)
                {
                    std::cout << "Working on TOKEN 40: " << token.tokenString << "\n";
                }
                node->child_4 = stat(depth);
                return node;
            }
            else
                error1(RIGHT_BRACKET_TK, token);
        }
        else
            error1(LEFT_BRACKET_TK, token);
    }
    else
        error1(LOOP_TK, token);
}

Node *label(unsigned int depth)
{
    if (deBug)
    {
        std::cout << "label"<< "\n";
    }
    depth++;
    Node *node = new Node("<label>", depth);
    if (token.tokenId == VOID_TK)
    {
        token = scanner(in_file, lineNum);
        if (deBug1)
        {
            std::cout << "Working on TOKEN 45: " << token.tokenString << "\n";
        }
        if (token.tokenId == ID_TK)
        {
            node->token1 = token;
            token = scanner(in_file, lineNum);
            if (deBug1)
            {
                std::cout << "Working on TOKEN 46: " << token.tokenString << "\n";
            }
            return node;
        }
        else
            error1(ID_TK, token);
    }
    else
        error1(VOID_TK, token);
}

void error1(tokens need, Token got)
{
    std::cout << "ERROR! " << std::endl;
    std::cout << "NEEDS TOKENS: " << tokes[need] << std::endl;
    std::cout << "GOT TOKENS: " << tokes[got.tokenId];
    std::cout << " LINE NUMBER: " << got.lineNum << std::endl;
    exit(EXIT_FAILURE);
}

void error2(tokens need_1, tokens need_2, Token got)
{
    std::cout << "ERROR!" << std::endl;
    std::cout << "NEEDS TOKENS: " << tokes[need_1] << " or " << tokes[need_2] << std::endl;
    std::cout << "GOT TOKENS: " << tokes[got.tokenId];
    std::cout << " LINE NUMBER: " << got.lineNum << std::endl;
    exit(EXIT_FAILURE);
}

void error3(tokens need_1, tokens need_2, tokens need_3, Token got)
{
    std::cout << "ERROR!" << std::endl;
    std::cout << "NEEDS TOKENS: " << tokes[need_1] << ", " << tokes[need_2];
    std::cout << ", or " << tokes[need_3] << std::endl;
    std::cout << "GOT TOKENS: " << tokes[got.tokenId];
    std::cout << " LINE NUMBER: " << got.lineNum << std::endl;
    exit(EXIT_FAILURE);
}
void error_mult(Token recieved)
{
    std::cout << "Token, NOT FOLLOWING THE RULE: " << tokes[recieved.tokenId];
    std::cout << " LINE NUMBER: " << recieved.lineNum << std::endl;
    exit(EXIT_FAILURE);
}

Node *gotoo(unsigned int depth)
{
    if (deBug)
    {
        std::cout << "gotoo" << "\n";
    }
    depth++;
    Node *node = new Node("<goto>", depth);
    if (token.tokenId == PROC_TK)
    {
        token = scanner(in_file, lineNum);
        if (deBug1)
        {
            std::cout << "Working on TOKEN 47: " << token.tokenString << "\n";
        }
        if (token.tokenId == ID_TK)
        {
            node->token1 = token;
            token = scanner(in_file, lineNum);
            if (deBug1)
            {
                std::cout << "Working on TOKEN 48: " << token.tokenString << "\n";
            }
            return node;
        }
        else
            error1(ID_TK, token);
    }
    else
        error1(PROC_TK, token);
}