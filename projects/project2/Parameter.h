#ifndef PARAMETER_H
#define PARAMETER_H

#include <string>

class Parameter {
public:
    std::string value;

    Parameter(const std::string& v);

    std::string toString() const;
};

#endif
