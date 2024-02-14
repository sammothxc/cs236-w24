#ifndef SCANNER_H
#define SCANNER_H

#include <vector>
#include "Token.h"

class Scanner {
private:
    std::vector<Token> tokens;
    int line;

    // Other private methods...

public:
    Scanner(const std::string& input);
    const std::vector<Token>& getTokens() const;
};

#endif
