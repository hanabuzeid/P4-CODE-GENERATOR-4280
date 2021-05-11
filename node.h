#ifndef NODE_H
#define NODE_H

#include "token.h"

struct Node
{
    std::string name;
    int depth;

    Token token1;
    Token token2;
    Token token3;
    Token token4;
    Token token5;
    Token token6;
    Node* child1; 
    Node* child2;
    Node* child3;
    Node* child_4;

    Node()
    {
        this->name = "";
        this->depth = 0;
        this->token1 = Token();
        this->token2 = Token();
        this->token3 = Token();
        this->token4 = Token();
        this->token5 = Token();
        this->token6 = Token();

        this->child1 = nullptr;
        this->child2 = nullptr;
        this->child3 = nullptr;
        this->child_4 = nullptr;
    }
    Node(std::string name, int depth)
    {
        this->name = name;
        this->depth = depth;
        this->token1 = Token();
        this->token2 = Token();
        this->token3 = Token();
        this->token4 = Token();
        this->token5 = Token();
        this->token6 = Token();
        this->child1 = nullptr;
        this->child2 = nullptr;
        this->child3 = nullptr;
        this->child_4 = nullptr;
    }
};

#endif