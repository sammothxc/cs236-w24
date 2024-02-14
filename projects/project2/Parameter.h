#ifndef PARAMETER_H
#define PARAMETER_H

#include <string>

class Parameter {
public:
    Parameter(TokenType type, std::string value) : type(type), value(value) {}

    std::string toString() const {
        return value;
    }

private:
    TokenType type;
    std::string value;
};

#endif