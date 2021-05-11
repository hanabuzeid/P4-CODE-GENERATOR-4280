#include "semantics.h"


#define MAX_STACK 100

Token stack[MAX_STACK];


int variableCount = 0, scope = 0; 

static int tempVarCount = 0;
static int labelCount = 0;
std::string temp_vars[MAX_STACK];
unsigned int currentTempVarsNum = 0;
unsigned int carryLabelsNum = 0;
unsigned int currentLabelsNum = 0;

vector<var_hold> VARIABLE(0);

int variable = 0;
int variableValue = 0;

void codeGen(Node* node, int index, std::string outputFileName) {

    file.open(outputFileName, std::ios_base::trunc);
    semantic_check(node, index);  
    file.close();
}


void push(Token tk) {
    if (variableCount >= MAX_STACK){
        std::cout << "Stack has reached MAX SIZE of 100 \n leaving..." << std::endl;
        exit(EXIT_FAILURE);
    }
    else{
        for (int i = scope; i < variableCount; i++){
            if (stack[i].tokenString == tk.tokenString){
                std::cout << "SEMANTICS ERROR... \'" << tk.tokenString << "\' already declared in this scope" << std::endl;
                exit(EXIT_FAILURE);
            }
        }

        stack[variableCount] = tk;

        file << "PUSH " << std::endl;
        variableCount++;

    }
}

void pop(int scope_start) {
    for (int i = variableCount; i > scope_start; i--){
        stack[i-1].tokenString = "";
       
    }
}

int topOfStackD(Token tk) {
    for (int i = variableCount; i >= scope; i--){
        if (stack[i].tokenString == tk.tokenString){
            int tos_distance = variableCount - 1 - i;
            return tos_distance;
        }
    }

    return -1;
}
bool var_exists(Token tk)
{
    for (int i = variableCount - 1; i > -1; i--){
        if (stack[i].tokenString == tk.tokenString) return true;
    }
    return false;
}

void semantic_check(Node* node, int index)
{
    
    if (node == nullptr)
        return;
   
    if (node->name == "<program>")//proj
    {
        int vars = 0;
        if (node->child1 != nullptr)
            semantic_check(node->child1, index);
        if (node->child2 != nullptr)
            semantic_check(node->child2, index);
        if (node->child3 != nullptr)
            semantic_check(node->child3, index);
        if (node->child_4 != nullptr)
            semantic_check(node->child_4, index);

        file << "STOP" << std::endl;


        for (unsigned int i = 0; i < VARIABLE.size(); i++) {
                file << VARIABLE[i].ID << " " << VARIABLE[i].value << std::endl;
        }

        for (int i = 0; i < currentTempVarsNum; i++)
        {
            
                file << "T" << i << " 0\n";
        }


    }


    else if (node->name == "<vars>") //var
    {
        int tos_distance = topOfStackD(node->token2);
        scope = variableCount;

        if (tos_distance == -1 || tos_distance > index){
            var_hold tempVar;
            tempVar.ID = node->token2.tokenString;
            tempVar.value = node->token4.tokenString;
            VARIABLE.resize(++variable, tempVar);
            push(node->token2);
            index++;
        }
        else if (tos_distance < index){
            error_declared(node->token2.tokenString);
            exit(EXIT_FAILURE);
        }

        if (node->child1 != nullptr)
            semantic_check(node->child1, index);
    }


    else if (node->name == "<block>"){ //block
        unsigned int vars = 0;
        scope = variableCount;

        if (node->child1 != nullptr)
            semantic_check(node->child1, index);
        if (node->child2 != nullptr)
            semantic_check(node->child2, index);
        if (node->child3 != nullptr)
            semantic_check(node->child3, index);
        if (node->child_4 != nullptr)
            semantic_check(node->child_4, index);
        pop(scope);
    }


    else if (node->name == "<expr>"){//expression
        if (node->token1.tokenId == MINUS_TK){
            int vars_num = currentTempVarsNum++;
            file << "SUB T" << vars_num << std::endl;
            if (node->child1 != nullptr)
                semantic_check(node->child1, index);
            if (node->child2 != nullptr)
                semantic_check(node->child2, index);
            if (node->child3 != nullptr)
                semantic_check(node->child3, index);
            if (node->child_4 != nullptr)
                semantic_check(node->child_4, index);
        }
        else if (node->child1 != nullptr)
            semantic_check(node->child1, index);
    }


    else if (node->name == "<N>"){
        int vars_num = currentTempVarsNum++;
        file << "STORE T" << vars_num << std::endl;
        if (node->token1.tokenId == SLASH_TK || node->token1.tokenId == ASTERISK_TK){
            
            
            
            if (node->token1.tokenId == SLASH_TK) {// multi and div
                file << "DIV T" << vars_num << std::endl;
            }
            else if (node->token1.tokenId == ASTERISK_TK) {
                file << "MULT T" << vars_num << std::endl;
            }
            if (node->child1 != nullptr)
                semantic_check(node->child1, index);
            if (node->child2 != nullptr)
                semantic_check(node->child2, index);
            if (node->child3 != nullptr)
                semantic_check(node->child3, index);
            if (node->child_4 != nullptr)
                semantic_check(node->child_4, index);
        }
        else if (node->child1 != nullptr){
            
            semantic_check(node->child1, index);
            
        }
            
    }


    else if (node->name == "<M>"){//
        if (node->token1.tokenId == ASTERISK_TK){
            if (node->child1 != nullptr)
                semantic_check(node->child1, index);
            if (node->child2 != nullptr)
                semantic_check(node->child2, index);
            if (node->child3 != nullptr)
                semantic_check(node->child3, index);
            if (node->child_4 != nullptr)
                semantic_check(node->child_4, index);
            file << "MULT -1" << std::endl;
        }
        else if (node->child1 != nullptr)
            semantic_check(node->child1, index);
    }


    else if (node->name == "<A>"){
        if (node->token1.tokenId == PLUS_TK){
            int vars_num = currentTempVarsNum++;
            file << "ADD T" << vars_num << std::endl;
            if (node->child1 != nullptr)
                semantic_check(node->child1, index);
            if (node->child2 != nullptr)
                semantic_check(node->child2, index);
            if (node->child3 != nullptr)
                semantic_check(node->child3, index);
            if (node->child_4 != nullptr)
                semantic_check(node->child_4, index);

        }
        else if (node->child1 != nullptr)
            semantic_check(node->child1, index);
    }


    else if (node->name == "<R>"){
        if (node->token1.tokenId == ID_TK){
            if (!var_exists(node->token1)){
                error_declared(node->token1.tokenString);
                exit(EXIT_FAILURE);
            }
            file << "LOAD " << node->token1.tokenString << std::endl;
        }
        if (node->token1.tokenId == INT_TK) {
            file << "LOAD " << node->token1.tokenString << std::endl;
        }
        else if (node->child1 != nullptr) {
            
            semantic_check(node->child1, index);
        }
    }

    else if (node->name == "<out>")
    {
        if (node->child1 != nullptr)
            semantic_check(node->child1, index);
        std::string temp_var = get_temp_var();
        file << "STORE " << temp_var << "\n";
        file << "WRITE " << temp_var << "\n";
    }

    else if (node->name == "<in>"){
        if (!var_exists(node->token2)){
            error_declared(node->token1.tokenString);
            exit(EXIT_FAILURE);
        }
        else {
            int var_location = var_exists(node->token1);
            if (var_location == -1)
            {
                std::cout << "ERROR: " << node->token1.tokenString << " already declared in this scope" << std::endl;
                exit(EXIT_FAILURE);
            }
//code provided in notes lecture
            std::string temp_var = get_temp_var();
            file << "\t\tREAD " << temp_var << "\n";
            file << "\t\tLOAD " << temp_var << "\n";
            file << "\t\tSTACKW " << var_location << "\n";
        }
    }

    else if (node->name == "<loop>") {
        int loop_num = currentLabelsNum++;
        int vars_num = currentTempVarsNum++;

        file << "L" << loop_num << ": NOOP" << std::endl;
        semantic_check(node->child1, index);
        file << "STORE T" << vars_num << std::endl;
        semantic_check(node->child3, index);
        file << "SUB T" << vars_num << std::endl;

        int exit_loop_num = currentLabelsNum;
        carryLabelsNum = currentLabelsNum++;
        semantic_check(node->child2, index);
        semantic_check(node->child_4, index);
        file << "BR L" << loop_num << std::endl;
        file << "L" << exit_loop_num << ": NOOP" << std::endl;
    }


    else if (node->name == "<assign>"){
        if (!var_exists(node->token2)){
            error_declared(node->token2.tokenString);
            exit(EXIT_FAILURE);
        }
        if (node->child1 != nullptr) {
            file << "STORE " << node->token2.tokenString << std::endl;
            semantic_check(node->child1, index);

        }
    }
    else {
    if (node->child1 != nullptr)
        semantic_check(node->child1, index);
    if (node->child2 != nullptr)
        semantic_check(node->child2, index);
    if (node->child3 != nullptr)
        semantic_check(node->child3, index);
    if (node->child_4 != nullptr)
        semantic_check(node->child_4, index);
    }
}

void check_children(Node* node, int index)
{
    if (node->child1 != nullptr)
        semantic_check(node->child1, index);
    if (node->child2 != nullptr)
        semantic_check(node->child2, index);
    if (node->child3 != nullptr)
        semantic_check(node->child3, index);
    if (node->child_4 != nullptr)
        semantic_check(node->child_4, index);
}

void error_declared(std::string tokenString){
    std::cout << "SEMANTICS ERROR ! \'" << tokenString << "\' already declared in this scope." << std::endl;
}

void show_stack() {
    std::cout << "Stack: ";
    int i;
    for (i = 0; i < 100; i++) {
        std::cout << stack[i].tokenString << "";
        if (stack[i].tokenString == "")break;
    }

    std::cout << std::endl;
}

std::string get_temp_var()
{
    std::string temp_var = "T" + std::to_string(tempVarCount + 1);
    temp_vars[tempVarCount] = temp_var;
    tempVarCount++;
    return temp_var;
}