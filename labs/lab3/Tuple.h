#pragma once
#include <vector>
#include <string>
#include <sstream>
#include "Scheme.h"

using namespace std;

class Tuple {
private:
    vector<string> values;

public:
    Tuple(vector<string> values) : values(values) {}

    unsigned size() const {
        return values.size();
    }

    const string& at(int index) const {
        return values.at(index);
    }

    bool operator<(const Tuple& t) const {
        return values < t.values;
    }

    string toString(const Scheme& scheme) const {
        stringstream out;
        for (size_t i = 0; i < scheme.size(); ++i) {
            out << scheme.at(i) << "='" << values.at(i) << "'";
            if (i < scheme.size() - 1) {
                out << ", ";
            }
        }
        return out.str();
    }
};