#include <iostream>
#include "./Lexer-folder/Lexer.hpp"
#include <unordered_set>
#include <string>
int main() {
    std::unordered_set<std::string> keywordSet = {"true", "false", "integer", "if", "else", "fi", "wihle", "return", "get", "put"};
    std::unordered_set<std::string> operatorSet = {"==", "!=", ">", "<", ">=", "<=", "+", "-", "*", "/"};
    // TODO: implement file I/O for the rat25F text file
    std::string sample = "true";
    std::cout << keywordSet.count(sample);
    return 0;
}