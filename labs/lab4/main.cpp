#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Scheme {
public:
    Scheme(const vector<string>& names) : names(names) {}
    size_t size() const { return names.size(); }
    string at(size_t index) const { return names.at(index); }
private:
    vector<string> names;
};

class Tuple {
public:
    Tuple(const vector<string>& values) : values(values) {}
    string at(size_t index) const { return values.at(index); }
private:
    vector<string> values;
};

class Relation {
public:
    static bool joinable(const Scheme& leftScheme, const Scheme& rightScheme,
                         const Tuple& leftTuple, const Tuple& rightTuple) {
        for (unsigned leftIndex = 0; leftIndex < leftScheme.size(); leftIndex++) {
            const string& leftName = leftScheme.at(leftIndex);
            const string& leftValue = leftTuple.at(leftIndex);
            cout << "left name: " << leftName << " value: " << leftValue << endl;

            for (unsigned rightIndex = 0; rightIndex < rightScheme.size(); rightIndex++) {
                const string& rightName = rightScheme.at(rightIndex);
                const string& rightValue = rightTuple.at(rightIndex);
                cout << "right name: " << rightName << " value: " << rightValue << endl;

                // Check if tuples are joinable based on some condition
                if (leftValue == rightValue) {
                    return true;
                }
            }
        }
        return false;
    }
};

int main() {
    Scheme scheme1({ "A", "B" });
    Scheme scheme2({ "B", "C" });

    Tuple tuple1({ "'1'", "'2'" });
    Tuple tuple2({ "'3'", "'4'" });

    Scheme scheme3({ "X", "Y" });
    Scheme scheme4({ "X", "Y", "Z" });

    Tuple tuple3({ "'1'", "'4'" });
    Tuple tuple4({ "'1'", "'2'", "'4'" });

    cout << Relation::joinable(scheme1, scheme2, tuple1, tuple2) << endl;
    cout << Relation::joinable(scheme2, scheme3, tuple1, tuple2) << endl;
    cout << Relation::joinable(scheme3, scheme4, tuple1, tuple4) << endl;
    cout << Relation::joinable(scheme3, scheme4, tuple3, tuple4) << endl;

    return 0;
}
