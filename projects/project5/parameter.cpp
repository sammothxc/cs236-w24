// ADDED IN PROJECT 2
#include "parameter.h"

void Parameter::SetParam(string Toke){
    theParameter = Toke;
    return;
}
string Parameter::ToString(){
    return theParameter;
}