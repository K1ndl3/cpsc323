#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <string>
#include <fstream>
// for checker function isdigit() - returns 1 if true, 0 if false
// for checker function isalpha() - returns 1 if true, 0 if false
#include <cctype>
#include "./Lexer-folder/Lexer.hpp"
#include <iomanip> // for std::setw

//////////////////////////////////////////// Look Up Tables ////////////////////////////////////////////////
const std::unordered_set<std::string> keywordSet   =  {"true", "false", "integer", "if", "else", "fi", "while", "return", "get", "put"};
const std::unordered_set<std::string> operatorSet  =  {"==", "!=", ">", "<", ">=", "<=", "+", "-", "*", "/", "="};
const std::unordered_set<std::string> separatorSet =  {"(", ")","{","}",";",","};
std::unordered_map<int, std::vector<int>> FSMtable = { // With key = int, with value = std::vector<int>
    {1, {2, 3, 4}},
    {2, {5, 6, 4}},
    {3, {4, 3, 7}},
    {4, {4, 4, 4}},
    {5, {5, 6, 4}},
    {6, {5, 6, 4}},
    {7, {4, 8, 4}},
    {8, {4, 8, 4}}
};
/////////////////////////////////////////// custom checker functions ///////////////////////////////////////
int isOperator(const std::string& str) {
    return operatorSet.count(str);
}

int isSeparator(const std::string& str) {
    return separatorSet.count(str);
}


///////////////////////////////////////// struct for token ////////////////////////////////////////////////
struct Token {
    std::string type;
    std::string lexeme;
};

// Lexer function process by starting with firstChar, uses FSM to consume token,
// prints token info, and returns the token
Token lexer(std::ifstream& inputFile, char firstChar) {
    std::string lexeme(1, firstChar);
    int state = 1;

    // Categorizes into FSM input classes where 0 is letter, 1 is digit, 2 is dot, -1 is invalid
    auto categorize = [](char c) {
        if (std::isalpha(c)) return 0;
        else if (std::isdigit(c)) return 1;
        else if (c == '.') return 2;
        else return -1;
    };

    int charType = categorize(firstChar);

    // Prints error if first character is invalid
    if (charType == -1) return {"LEXICAL ERROR", lexeme};

    char ch;
    while(true) {
                
        char peek = inputFile.peek();
        int nextCharType = categorize(peek);
        // Check if FSM transition exists for current state and next char type
        if (nextCharType != -1 && FSMtable.count(state)) {
            int nextState = FSMtable[state][nextCharType];
            
            // If next state is invalid, stop consuming characters
            if (nextState == 4) break;
            state = nextState;
            inputFile.get(ch);
            lexeme += ch;

        } else break; // No valid transition for next char, end token
    }

    // if lexeme is of size 1 that means we only have 1 character
    if (lexeme.size() == 1) {
        int arrIndex = categorize(lexeme[0]);
        state = FSMtable[state][arrIndex];
    }
    // Classify and print token based on final FSM state
    if (state == 2 || state == 5 || state == 6) {
        if (keywordSet.count(lexeme)) return {"KEYWORD", lexeme};
        return {"IDENTIFIER", lexeme};
    }
    if (state == 3) return {"INTEGER", lexeme};
    if (state == 8) return {"REAL", lexeme};

    // Default case: invalid token
    return {"LEXICAL ERROR", lexeme};
}

// Function to print tokens in a formatted table
void printToken(const Token& token) {
    std::cout << std::left << std::setw(15) << token.type
              << std::setw(15) << token.lexeme << '\n';
}

int main() {
    std::cout << "Lexer for rat24F starting\n";

    // Taking input from input.txt and storing into inputFile
    std::ifstream inputFile("input.txt");
    if (!inputFile) {
        std::cout << "[ERROR] -> cannot open file " << "'input.txt'\n";
        return 1;
    }

    // Input file successfully loaded
    std::cout << "[SUCCESS] -> file opened\n";
    std::cout << "Running lexer...\n\n\n\n\n";
    std::cout << "============================\n";

    // Print table header
    std::cout << std::left << std::setw(15) << "TOKEN TYPE"
              << std::setw(15) << "LEXEME" << '\n';
    std::cout << "============================\n";

    // Iterate through the inputFile using .get() and run lexer()
    char ch;
    while (inputFile.get(ch)) {
        // Skip white spaces
        if (std::isspace(ch)) continue;

        std::string lexeme(1, ch);

        // Check for operators with multi-characters
        if (ch == '=' || ch == '!' || ch == '<' || ch == '>') {
            int np = inputFile.peek();
            if (np != EOF && static_cast<char>(np) == '=') {
                char next;
                inputFile.get(next);
                lexeme.push_back(next);
            }
        }

        if (isOperator(lexeme)) {
            std::cout << std::left << std::setw(15) << "OPERATOR"
                      << std::setw(15) << lexeme << '\n';
            continue;
        }

        if (isSeparator(lexeme)) {
            std::cout << std::left << std::setw(15) << "SEPARATOR"
                      << std::setw(15) << lexeme << '\n';
            continue;
        }

        Token token = lexer(inputFile, ch);
        printToken(token);
    }

    return 0;
}