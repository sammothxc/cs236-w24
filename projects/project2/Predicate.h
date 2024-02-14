#ifndef PREDICATE_H
#define PREDICATE_H

#include <string>
#include <vector>
#include "Parameter.h"

class Predicate {
public:
    Predicate(std::string name) : name(name) {}

    void addParameter(Parameter parameter) {
        parameters.push_back(parameter);
    }

    std::string toString() const {
        std::string result = name + "(";
        for (size_t i = 0; i < parameters.size(); ++i) {
            result += parameters[i].toString();
            if (i != parameters.size() - 1) {
                result += ",";
            }
        }
        result += ")";
        return result;
    }

private:
    std::string name;
    std::vector<Parameter> parameters;
};


#endif