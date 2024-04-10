// ADDED IN PROJECT 3
#pragma once
#include <string>
#include <vector>
#include <iostream>
using namespace std;

class Tuple : public vector<string> {
    bool beenOutputted;
    vector <string> values;
    string At (int num) {
        return values.at(num);
    }
};