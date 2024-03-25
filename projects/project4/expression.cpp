// ADDED IN PROJECT 2
#include "expression.h"

void Expression::SetRight(Parameter right) {
    rightParam = right;
}
void Expression:: SetOperator(Parameter oper) {
    Operator = oper;
}
void Expression::SetLeft(Parameter left) {
    leftParam = left;
}
string Expression::GetExpression(){
    string Express;
    Express = rightParam.ToString() + Operator.ToString() + leftParam.ToString();
    return Express;
}
string Expression::ToString() {
    cout << "Expression to String" << endl;
    string stringy = "(" + rightParam.ToString() + Operator.ToString() + leftParam.ToString() + ")";
    return stringy;
}