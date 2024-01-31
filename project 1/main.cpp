#include <iostream>
#include <fstream>
#include "Token.h"
#include "Scanner.h"

int main(int argc, char* argv[]) {
    // Check if the correct number of command-line arguments is provided
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
        return 1;
    }

    // Open the input file
    std::ifstream inputFile(argv[1]);
    if (!inputFile.is_open()) {
        std::cerr << "Error: Unable to open input file: " << argv[1] << std::endl;
        return 1;
    }

    // Read the input file into a string
    std::string inputString((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());

    // Create a scanner object
    Scanner scanner(inputString);

    // Tokenize input and print tokens
    Token token = scanner.scanToken();
    int tokenCount = 0;
    // Workaround to print EOF token
    do {
        std::cout << token.toString() << std::endl;
        if (token.getType() == EOF_TOKEN) {
            std::cout << "EOFFFFF boiiii" << std::endl; // Slowly losing my mind
        }
        tokenCount++;
        token = scanner.scanToken();
    } while (token.getType() != EOF_TOKEN);


    // Print total token count
    std::cout << "Total Tokens = " << tokenCount << std::endl;

    return 0;
}
