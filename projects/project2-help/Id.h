#pragma once
#include <string>
#include <vector>
#include "Parameter.h"

using namespace std;

class Id : public Parameter {

public:
  Id(string str){
    value = str;
  }
  ~Id(){}

  string getValue(){
    return value;
  }

  string toString(){
    string output = "";
    output += value;
    return output;
  }

private:
  string value;
};
