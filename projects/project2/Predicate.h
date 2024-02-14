

class Predicate {
public:
    string name;
    vector<Parameter> parameters;

    Predicate(const string& n, const vector<Parameter>& p) : name(n), parameters(p) {}

    string toString() const {
        string result = name + "(";
        for (size_t i = 0; i < parameters.size(); ++i) {
            result += parameters[i].toString();
            if (i != parameters.size() - 1) result += ",";
        }
        result += ")";
        return result;
    }
};