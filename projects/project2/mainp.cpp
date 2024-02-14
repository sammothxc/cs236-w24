#include <iostream>
#include <vector>
#include <string>
#include <set>

using namespace std;

// Forward declaration of classes
class Predicate;
class Parameter;

// Class to represent a rule in the Datalog program
class Rule {
public:
    Predicate* headPredicate;
    vector<Predicate*> bodyPredicates;

    Rule(Predicate* headPred) : headPredicate(headPred) {}
    ~Rule() {
        delete headPredicate;
        for (auto& pred : bodyPredicates)
            delete pred;
    }
    string toString() const {
        string result = headPredicate->toString() + " :- ";
        for (size_t i = 0; i < bodyPredicates.size(); ++i) {
            if (i != 0)
                result += ",";
            result += bodyPredicates[i]->toString();
        }
        result += ".";
        return result;
    }
};

// Class to represent a predicate in the Datalog program
class Predicate {
public:
    string name;
    vector<Parameter*> parameters;

    Predicate(const string& n) : name(n) {}
    ~Predicate() {
        for (auto& param : parameters)
            delete param;
    }
    string toString() const {
        string result = name + "(";
        for (size_t i = 0; i < parameters.size(); ++i) {
            if (i != 0)
                result += ",";
            result += parameters[i]->toString();
        }
        result += ")";
        return result;
    }
};

// Class to represent a parameter in the Datalog program
class Parameter {
public:
    string value;
    bool isString;

    Parameter(const string& v, bool isStr) : value(v), isString(isStr) {}
    string toString() const {
        if (isString)
            return "'" + value + "'";
        else
            return value;
    }
};

// Class to represent the entire Datalog program
class DatalogProgram {
public:
    vector<Predicate*> schemes;
    vector<Predicate*> facts;
    vector<Rule*> rules;
    vector<Predicate*> queries;
    set<string> domain;

    ~DatalogProgram() {
        for (auto& scheme : schemes)
            delete scheme;
        for (auto& fact : facts)
            delete fact;
        for (auto& rule : rules)
            delete rule;
        for (auto& query : queries)
            delete query;
    }

    void addToDomain(const string& value) {
        domain.insert(value);
    }

    void print() const {
        cout << "Success!" << endl;
        printList("Schemes", schemes);
        printList("Facts", facts);
        printList("Rules", rules);
        printList("Queries", queries);
        cout << "Domain(" << domain.size() << "):" << endl;
        for (const auto& value : domain)
            cout << "  " << value << endl;
    }

private:
    template<typename T>
    void printList(const string& label, const vector<T*>& list) const {
        cout << label << "(" << list.size() << "):" << endl;
        for (const auto& item : list)
            cout << "  " << item->toString() << endl;
    }
};

// Forward declaration of parsing functions
DatalogProgram parseDatalogProgram(istream& input);
Predicate* parseScheme(istream& input);
Predicate* parseFact(istream& input);
Rule* parseRule(istream& input);
Predicate* parseQuery(istream& input);
Predicate* parseHeadPredicate(istream& input);
Predicate* parsePredicate(istream& input);
Parameter* parseParameter(istream& input);

// Main parsing function to parse the entire Datalog program
DatalogProgram parseDatalogProgram(istream& input) {
    DatalogProgram program;

    string section;
    while (input >> section) {
        if (section == "Schemes:")
            while (true) {
                Predicate* scheme = parseScheme(input);
                if (!scheme)
                    break;
                program.schemes.push_back(scheme);
            }
        else if (section == "Facts:")
            while (true) {
                Predicate* fact = parseFact(input);
                if (!fact)
                    break;
                program.facts.push_back(fact);
                // Add strings from facts to domain
                for (auto& param : fact->parameters)
                    if (param->isString)
                        program.addToDomain(param->value);
            }
        else if (section == "Rules:")
            while (true) {
                Rule* rule = parseRule(input);
                if (!rule)
                    break;
                program.rules.push_back(rule);
            }
        else if (section == "Queries:")
            while (true) {
                Predicate* query = parseQuery(input);
                if (!query)
                    break;
                program.queries.push_back(query);
            }
    }

    return program;
}

// Parse a scheme
Predicate* parseScheme(istream& input) {
    string name;
    if (!(input >> name) || name == "Facts:")
        return nullptr;

    Predicate* scheme = new Predicate(name);

    string token;
    if (!(input >> token) || token != "(")
        throw runtime_error("Expected '(' after scheme name");

    while (true) {
        Parameter* param = parseParameter(input);
        if (!param)
            break;
        scheme->parameters.push_back(param);
    }

    if (!(input >> token) || token != ")")
        throw runtime_error("Expected ')' after scheme parameters");

    return scheme;
}

// Parse a fact
Predicate* parseFact(istream& input) {
    string name;
    if (!(input >> name) || name == "Rules:")
        return nullptr;

    Predicate* fact = new Predicate(name);

    string token;
    if (!(input >> token) || token != "(")
        throw runtime_error("Expected '(' after fact name");

    while (true) {
        string value;
        if (!(input >> value))
            throw runtime_error("Expected value in fact");
        bool isString = false;
        if (value.front() == '\'') {
            if (value.back() != '\'')
                throw runtime_error("Unclosed string in fact");
            value = value.substr(1, value.size() - 2);
            isString = true;
        }
        fact->parameters.push_back(new Parameter(value, isString));

        if (!(input >> token))
            throw runtime_error("Expected ',' or '.' in fact");
        if (token == ",")
            continue;
        else if (token == ")")
            break;
        else
            throw runtime_error("Expected ',' or ')' in fact");
    }

    if (!(input >> token) || token != ".")
        throw runtime_error("Expected '.' after fact");

    return fact;
}

// Parse a rule
Rule* parseRule(istream& input) {
    Predicate* headPred = parseHeadPredicate(input);

    string token;
    if (!(input >> token) || token != ":-")
        throw runtime_error("Expected ':-' after head predicate in rule");

    Rule* rule = new Rule(headPred);

    while (true) {
        Predicate* bodyPred = parsePredicate(input);
        if (!bodyPred)
            break;
        rule->bodyPredicates.push_back(bodyPred);
    }

    if (!(input >> token) || token != ".")
        throw runtime_error("Expected '.' after rule body");

    return rule;
}

// Parse a query
Predicate* parseQuery(istream& input) {
    Predicate* query = parsePredicate(input);

    string token;
    if (!(input >> token) || token != "?")
        throw runtime_error("Expected '?' at the end of query");

    return query;
}

// Parse a head predicate
Predicate* parseHeadPredicate(istream& input) {
    string name;
    if (!(input >> name))
        throw runtime_error("Expected predicate name");

    Predicate* headPred = new Predicate(name);

    string token;
    if (!(input >> token) || token != "(")
        throw runtime_error("Expected '(' after head predicate name");

    while (true) {
        Parameter* param = parseParameter(input);
        if (!param)
            break;
        headPred->parameters.push_back(param);
    }

    if (!(input >> token) || token != ")")
        throw runtime_error("Expected ')' after head predicate parameters");

    return headPred;
}

// Parse a predicate
Predicate* parsePredicate(istream& input) {
    string name;
    if (!(input >> name))
        return nullptr;

    Predicate* pred = new Predicate(name);

    string token;
    if (!(input >> token) || token != "(")
        throw runtime_error("Expected '(' after predicate name");

    while (true) {
        Parameter* param = parseParameter(input);
        if (!param)
            break;
        pred->parameters.push_back(param);
    }

    if (!(input >> token) || token != ")")
        throw runtime_error("Expected ')' after predicate parameters");

    return pred;
}

// Parse a parameter
Parameter* parseParameter(istream& input) {
    string token;
    if (!(input >> token))
        return nullptr;

    if (token == ")")
        return nullptr;
    else if (token.front() == '\'') {
        if (token.back() != '\'')
            throw runtime_error("Unclosed string in parameter");
        return new Parameter(token.substr(1, token.size() - 2), true);
    } else
        return new Parameter(token, false);
}

int main() {
    try {
        DatalogProgram program = parseDatalogProgram(cin);
        program.print();
    } catch (const exception& e) {
        cerr << "Failure!" << endl;
        cerr << "  (" << e.what() << ")" << endl;
    }
    return 0;
}
