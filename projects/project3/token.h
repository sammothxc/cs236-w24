#pragma once
#include <iostream>
#include <string>
#include <list>
#include <fstream>
#include <vector>
#include "Token.h"
using namespace std;

class Token {
public:
	string type;
	string value;
	int line;
	string toString() {
		return ("(" + type + "," + value + "," + to_string(line) + ")");
	}
	string getValue() {
		return value.substr(1, value.size() - 2);
	}
	string getTokenType() {
		return  type;
	}
	Token(string typeOf, string values, int lineNum) {
		type = typeOf;
		value = values;
		line = lineNum;
	}
	Token() {}
};
