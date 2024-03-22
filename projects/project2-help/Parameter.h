#pragma once
#include <string>
#include <vector>

using namespace std;

class Parameter {
public:
  Parameter(){}
  ~Parameter(){}

  virtual string toString(){
    return "";
  }

  virtual int getIndex(){
    return 0;
  }

private:
};
