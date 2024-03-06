#include <iostream>
#include "Scheme.h"
#include "Tuple.h"
#include "Relation.h"

int main() {

    vector<string> names = { "ID", "Name", "Major" };
    Scheme scheme(names);

    vector<string> values[] = {
        {"'42'", "'Ann'", "'CS'"},
        {"'32'", "'Bob'", "'CS'"},
        {"'64'", "'Ned'", "'EE'"},
        {"'16'", "'Jim'", "'EE'"},
    };

    for (auto& value : values) {
        Tuple tuple(value);
        cout << tuple.toString(scheme) << endl;
    }

    Relation relation("student", scheme);

    for (auto& value : values) {
        Tuple tuple(value);
        relation.addTuple(tuple);
    }

    cout << "relation:" << endl;
    cout << relation.toString() << endl;

    Relation result = relation.select(2, "'CS'");

    cout << "select Major='CS' result:" << endl;
    cout << result.toString() << endl;

    return 0;
}