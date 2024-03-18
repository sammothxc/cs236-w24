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
    string toString(const Scheme& scheme) const {
        string str;
        for (size_t i = 0; i < values.size(); ++i) {
            str += scheme.at(i) + "=" + values[i];
            if (i < values.size() - 1) {
                str += ", ";
            }
        }
        return str;
    }
private:
    vector<string> values;
};

class Relation {
public:
    Relation(const string& name, const Scheme& scheme) : name(name), scheme(scheme) {}

    void addTuple(const Tuple& tuple) {
        tuples.push_back(tuple);
    }

    Relation join(const Relation& right) {
        const Relation& left = *this;
        Relation result("joined", left.scheme); // Assuming the scheme of the result will be same as left

        for (const auto& leftTuple : left.tuples) {
            cout << "left tuple: " << leftTuple.toString(left.scheme) << endl;

            for (const auto& rightTuple : right.tuples) {
                cout << "right tuple: " << rightTuple.toString(right.scheme) << endl;
            }
        }

        return result;
    }

private:
    string name;
    Scheme scheme;
    vector<Tuple> tuples;
};

int main() {
    Relation studentRelation("students", Scheme({ "ID", "Name", "Major" }));
    vector<string> studentValues[] = {
        { "'42'", "'Ann'", "'CS'" },
        { "'64'", "'Ned'", "'EE'" },
    };

    for (auto& value : studentValues)
        studentRelation.addTuple(Tuple(value));

    //studentRelation.join(studentRelation);

    Relation courseRelation("courses", Scheme({ "ID", "Course" }));
    vector<string> courseValues[] = {
        { "'42'", "'CS 100'" },
        { "'32'", "'CS 232'" },
    };

    for (auto& value : courseValues)
        courseRelation.addTuple(Tuple(value));

    studentRelation.join(courseRelation);

    return 0;
}