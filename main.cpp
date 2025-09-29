#include <iostream>
#include "./Lexer-folder/Lexer.hpp"
#include <unordered_set>
#include <string>
#include <fstream>

//////////////////////////////////////////// Look Up Tables ////////////////////////////////////////////////
const std::unordered_set<std::string> keywordSet = {"true", "false", "integer", "if", "else", "fi", "while", "return", "get", "put"};
const std::unordered_set<std::string> operatorSet = {"==", "!=", ">", "<", ">=", "<=", "+", "-", "*", "/"};


int main() {
    std::cout << "Lexer for rat24F starting\n";
    
    // Taking input from input.txt and storing into inputFile
    std::ifstream inputFile("input.txt");
    if (!inputFile) {
        std::cout << "[ERROR] -> cannot open file " << "'input.txt'\n";
        return 1;
    }
    
    // Input file sucessfully loaded
    std::cout << "[SUCCESS] -> file opened";
    std::cout << "Running lexer...\n";
    

    // iterate through the inputFile using .get() and run lexer()
    char ch;
    while (inputFile.get(ch)) {
        std::cout << ch << '\n';
    }

    return 0;
}