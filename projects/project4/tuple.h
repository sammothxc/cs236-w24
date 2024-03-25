#pragma once
using namespace std;
#include <string>
#include <vector>
#include <iostream>

class Tuple : public vector<string> {
    bool beenOutputted;
    vector <string> values;
    string At (int num) {
        return values.at(num);
    }
};