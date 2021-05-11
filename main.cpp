#include <iostream>
#include <string>
#include <fstream>
#include "parser.h"
#include "semantics.h"

std::ofstream file;

int main(int argc, char **argv)
{
    std::string fileName;

    static std::string outFileName;

    if (argc == 1)
    {
        std::string user;
        std::ofstream file;
        fileName = "stdin.temp";

        file.open(fileName, std::ios::trunc);

        std::string string = "";

        std::cout << "Pressing Enter will simulate EOF" << std::endl;

        do
        {
            std::cout << std::endl << "USER INPUT: ";

            getline(std::cin, user);
            file << user << "\n";
        } while (!user.empty());

        file.close();
        in_file.open(fileName);
    }
    else if (argc == 2)
    {
        fileName = argv[1];
        outFileName = fileName + ".asm";
        fileName += ".sp21";

        std::ofstream outfile;
        outfile.open(fileName, std::ios_base::app);
        outfile << " ";
        in_file.open(fileName);
    }

    else
    {
        std::cout << "EXCEEDED ARGUMENTS GIVEN" << std::endl;
        exit(EXIT_FAILURE);
    }

    if (in_file)
    {

        Node *root = parser();
        codeGen(root, 0, outFileName);
        in_file.close();
    }
    else
    { 
        std::cout << "ERROR, CANT OPEN FILE " << fileName << " FOR READING" << std::endl;
        in_file.close();
        exit(EXIT_FAILURE);
    }
    return 0;
}