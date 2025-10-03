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

//////////////////////////////////////////// Look Up Tables ////////////////////////////////////////////////
const std::unordered_set<std::string> keywordSet   =  {"true", "false", "integer", "if", "else", "fi", "while", "return", "get", "put"};
const std::unordered_set<std::string> operatorSet  =  {"==", "!=", ">", "<", ">=", "<=", "+", "-", "*", "/", "="};
const std::unordered_set<std::string> separatorSet =  {"(", ")","{","}",";",","};

/////////////////////////////////////////// custom checker functions ///////////////////////////////////////
int isOperator(const std::string& str) {
    return operatorSet.count(str);
}

int isSeparator(const std::string& str) {
    return separatorSet.count(str);
}

// TODO: Implement the hash table of the states (unordered map)
// With key = int, with value = std::vector<int>
std::unordered_map<int, std::vector<int>> FSMtable = {
    {1, {2, 3, 4}},
    {2, {5, 6, 4}},
    {3, {4, 3, 7}},
    {4, {4, 4, 4}},
    {5, {5, 6, 4}},
    {6, {5, 6, 4}},
    {7, {4, 8, 4}},
    {8, {4, 8, 4}}
};

// Structure for holding token information to pass between lexer and main
struct Token {
    std::string type;
    std::string lexeme;
};

// Lexer function process by starting with firstChar, uses FSM to consume token,
// prints token info, and returns the token
Token lexer(std::ifstream& inputFile, char firstChar) {
    std::string lexeme(1, firstChar);
    int state = 1;

    // anon
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
        // TODO: the lexer is not reading the current char but is instead entering the next state thru char's next by peek()
        
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

        // 
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

        Token token = lexer(inputFile, ch);
        std::cout << "[" << token.type << "] -> " << token.lexeme << '\n';
    }


    return 0;
}