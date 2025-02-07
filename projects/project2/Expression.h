#pragma once
#include "Token.h"
#include "Parameter.h"
#include <vector>

class Expression {
public:
    void SetRight(Parameter right);
    void SetOperator(Parameter oper);
    void SetLeft(Parameter left);
    string GetExpression();
    string ToString();
private:
    Parameter rightParam;
    Parameter Operator;
    Parameter leftParam;
};