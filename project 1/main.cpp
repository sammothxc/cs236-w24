#include <iostream>
#include <fstream>
#include "Token.h"
#include "Scanner.h"

int main() {
    // Open the input file
    std::ifstream inputFile("input.txt");
    if (!inputFile.is_open()) {
        std::cerr << "Error: Unable to open input file." << std::endl;
        return 1;
    }

    // Read the input file into a string
    std::string inputString((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());

    // Create a scanner object
    Scanner scanner(inputString);

    // Tokenize input and print tokens
    Token token;
    int tokenCount = 0;
    while ((token = scanner.scanToken()).getType() != EOF_TOKEN) {
        std::cout << token.toString() << std::endl;
        tokenCount++;
    }

    // Print total token count
    std::cout << "Total Tokens = " << tokenCount << std::endl;

    return 0;
}
