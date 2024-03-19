#pragma once
#include <string>
#include <vector>
#include "Parameter.h"

using namespace std;

class String : public Parameter {

public:
  String(string str){
    value = str;
  }
  ~String(){}

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
