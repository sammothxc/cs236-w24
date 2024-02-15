#pragma once
#include <string>
#include <vector>
#include "Token.h"
#include "Parameter.h"

class Predicate {
private:
    std::string name;
    std::vector<std::string> parameters;

public:
    Predicate(const std::string& name) : name(name) {}

    void addParameter(const std::string& param) {
        parameters.push_back(param);
    }

    std::string toString() const {
        std::string result = name + "(";
        for (size_t i = 0; i < parameters.size(); ++i) {
            if (i > 0) result += ",";
            result += parameters[i];
        }
        result += ")";
        return result;
    }
};
