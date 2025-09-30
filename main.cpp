#include <iostream>
#include <unordered_set>
#include <string>
#include <fstream>
// for checker function isdigit() - returns 1 if true, 0 if false
// for checker function isalpha() - returns 1 if true, 0 if false
#include <cctype>
#include "./Lexer-folder/Lexer.hpp"

//////////////////////////////////////////// Look Up Tables ////////////////////////////////////////////////
const std::unordered_set<std::string> keywordSet   =  {"true", "false", "integer", "if", "else", "fi", "while", "return", "get", "put"};
const std::unordered_set<std::string> operatorSet  =  {"==", "!=", ">", "<", ">=", "<=", "+", "-", "*", "/"};
const std::unordered_set<std::string> separatorSet =  {"(", ")","{","}",";",","};

/////////////////////////////////////////// custom checker functions ///////////////////////////////////////
int isOperator(std::string str) {
    return operatorSet.count(str);
}

int isSeparator(std::string str) {
    return separatorSet.count(str);
}

int main() {
    std::cout << "Lexer for rat24F starting\n";
    
    // Taking input from input.txt and storing into inputFile
    std::ifstream inputFile("input.txt");
    if (!inputFile) {
        std::cout << "[ERROR] -> cannot open file " << "'input.txt'\n";
        return 1;
    }
    
    // Input file sucessfully loaded
    std::cout << "[SUCCESS] -> file opened\n";
    std::cout << "Running lexer...\n\n\n\n\n";
    std::cout << "============================\n";
    

    // iterate through the inputFile using .get() and run lexer()
    // this technically will be the lexer function but we can always wrap it in
    // a function after we finish
    char ch;
    while (inputFile.get(ch)) {
        // skips white spaces
        if (std::isspace(ch)) continue;

        std::string lexeme(1, ch);


        // checks for operators with multi-characters
        if (ch == '=' || ch == '!' || ch == '<' || ch == '>') {
            char next = inputFile.peek();
            if (next == '=') {
                inputFile.get(next);
                lexeme.push_back(next);
            } 
        }

        if(isOperator(lexeme)) {
            std::cout << "[OPERATOR] -> " << lexeme << '\n';
            continue;
        }

        if (isSeparator(lexeme)) {
            std::cout << "[SEPARATOR] -> " << lexeme << '\n';
            continue;
        }

        std::cout << "[LEXICAL ERROR: UNKNOWN SYMBOL] -> " << lexeme << '\n';
    }

    return 0;
}