#ifndef PREDICATE_H
#define PREDICATE_H

#include <string>
#include <vector>
#include "Parameter.h"

class Predicate {
public:
    std::string name;
    std::vector<Parameter> parameters;

    Predicate(const std::string& n, const std::vector<Parameter>& p);

    std::string toString() const;
};

#endif