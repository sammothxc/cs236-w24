// ADDED IN PROJECT 2
#pragma once
#include "string"
using namespace std;

class Parameter {
    public:
        void SetParam(string param);
        string ToString();
        void SetBool(bool trueOrFalse) {
            boolString = trueOrFalse;
        }
        bool IsString() {
            return boolString;
        }
        string theParameter;
        bool boolString;
};