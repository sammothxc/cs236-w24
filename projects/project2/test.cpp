#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <set>
#include <algorithm>

enum TokenType {
    COMMA,
    PERIOD,
    Q_MARK,
    LEFT_PAREN,
    RIGHT_PAREN,
    COLON,
    COLON_DASH,
    MULTIPLY,
    ADD,
    SCHEMES,
    FACTS,
    RULES,
    QUERIES,
    ID,
    STRING,
    COMMENT,
    UNDEFINED,
    EOF_TOKEN // Need to debug this
};

class Token {
private:
    TokenType type;
    std::string value;
    int line;
public:
    Token(TokenType type, const std::string& value, int line) : type(type), value(value), line(line) {}

    TokenType getType() const { return type; }

    std::string typeName() const {
        switch(type) {
            case COMMA: return "COMMA";
            case PERIOD: return "PERIOD";
            case Q_MARK: return "Q_MARK";
            case LEFT_PAREN: return "LEFT_PAREN";
            case RIGHT_PAREN: return "RIGHT_PAREN";
            case COLON: return "COLON";
            case COLON_DASH: return "COLON_DASH";
            case MULTIPLY: return "MULTIPLY";
            case ADD: return "ADD";
            case SCHEMES: return "SCHEMES";
            case FACTS: return "FACTS";
            case RULES: return "RULES";
            case QUERIES: return "QUERIES";
            case ID: return "ID";
            case STRING: return "STRING";
            case COMMENT: return "COMMENT";
            case UNDEFINED: return "UNDEFINED";
            case EOF_TOKEN: return "EOF";
            default: return "UNKNOWN";
        }
    }

    std::string toString() const {
        if (type == COMMENT) {
            return "(" + typeName() + ",\"#" + value + "\"," + std::to_string(line) + ")";
        } else {
            return "(" + typeName() + ",\"" + value + "\"," + std::to_string(line) + ")";
        }
    }
};

class Scanner {
private:
    std::string input;
    int line = 1; // Track current line number
public:
    Scanner(const std::string& input) : input(input) {}

    Token scanToken() {
        if (input.empty()) {
            return Token(EOF_TOKEN, "", line);
        }
        while (!input.empty()) {
            char c = input.front();
            input = input.substr(1);

            if (isspace(c)) {
                if (c == '\n') line++;
                continue;
            }

            switch (c) {
                case ',':
                    return Token(COMMA, ",", line);
                case '.':
                    return Token(PERIOD, ".", line);
                case '?':
                    return Token(Q_MARK, "?", line);
                case '(':
                    return Token(LEFT_PAREN, "(", line);
                case ')':
                    return Token(RIGHT_PAREN, ")", line);
                case ':':
                    if (!input.empty() && input.front() == '-') {
                        input = input.substr(1);
                        return Token(COLON_DASH, ":-", line);
                    }
                    return Token(COLON, ":", line);
                case '+':
                    return Token(ADD, "+", line);
                case '*':
                    return Token(MULTIPLY, "*", line);
                case '\'':
                    return scanString();
                case '#':
                    return scanComment();
                default:
                    if (isalpha(c)) {
                        return scanIdentifier(c);
                    }
                    return Token(UNDEFINED, std::string(1, c), line);
            }
        }
        return Token(EOF_TOKEN, "", line);
    }

    Token scanString() {
        std::string value;
        int startLine = line;
        value += '\'';
        while (!input.empty()) {
            char c = input.front();
            input = input.substr(1);
            if (c == '\'') {
                if (!input.empty() && input.front() == '\'') {
                    value += c;
                    input = input.substr(1);
                } else {
                    value += c;
                    return Token(STRING, value, startLine);
                }
            } else if (c == '\n') {
                line++;
                value += c;
            } else {
                value += c;
            }
        }
        return Token(UNDEFINED, value, startLine);
    }

    Token scanComment() {
        char c;
        std::string value;
        if (!input.empty() && input.front() == '|') {
            input = input.substr(1);
            while (!input.empty()) {
                c = input.front();
                input = input.substr(1);
                if (c == '|' && !input.empty() && input.front() == '#') {
                    input = input.substr(1);
                    return Token(COMMENT, value, line);
                } else {
                    if (c == '\n') line++;
                    value += c;
                }
            }
            return Token(UNDEFINED, value, line);
        } else {
            while (!input.empty() && input.front() != '\n') {
                value += input.front();
                input = input.substr(1);
            }
            return Token(COMMENT, value, line);
        }
    }

    Token scanIdentifier(char firstChar) {
        std::string value;
        value += firstChar;
        while (!input.empty() && (isalnum(input.front()) || input.front() == '_')) {
            value += input.front();
            input = input.substr(1);
        }
        if (value == "Schemes") return Token(SCHEMES, value, line);
        if (value == "Facts") return Token(FACTS, value, line);
        if (value == "Rules") return Token(RULES, value, line);
        if (value == "Queries") return Token(QUERIES, value, line);
        return Token(ID, value, line);
    }
};

class DatalogProgram {
private:
    std::vector<Predicate> schemes;
    std::vector<Predicate> facts;
    std::vector<Rule> rules;
    std::vector<Predicate> queries;
    std::set<std::string> domain;
    
public:
    void addScheme(const Predicate& scheme) {
        schemes.push_back(scheme);
    }

    void addFact(const Predicate& fact) {
        facts.push_back(fact);
    }

    void addRule(const Rule& rule) {
        rules.push_back(rule);
    }

    void addQuery(const Predicate& query) {
        queries.push_back(query);
    }

    void addToDomain(const std::string& value) {
        domain.insert(value);
    }

    std::string toString() const {
        std::string result = "Schemes(" + std::to_string(schemes.size()) + "):\n";
        for (const auto& scheme : schemes) {
            result += "  " + scheme.toString() + "\n";
        }
        result += "Facts(" + std::to_string(facts.size()) + "):\n";
        for (const auto& fact : facts) {
            result += "  " + fact.toString() + "\n";
        }
        result += "Rules(" + std::to_string(rules.size()) + "):\n";
        for (const auto& rule : rules) {
            result += "  " + rule.toString() + "\n";
        }
        result += "Queries(" + std::to_string(queries.size()) + "):\n";
        for (const auto& query : queries) {
            result += "  " + query.toString() + "\n";
        }
        result += "Domain(" + std::to_string(domain.size()) + "):\n";
        for (const auto& value : domain) {
            result += "  " + value + "\n";
        }
        return result;
    }
};

class Parameter {
private:
    Token parameter;

public:
    // Constructor, Access Methods, Update Methods, toString, etc.
};

class Predicate {
private:
    std::string name;
    std::vector<std::string> parameters;

public:
    Predicate(const std::string& name) : name(name) {}

    void addParameter(const std::string& param) {
        parameters.push_back(param);
    }

    std::string toString() const {
        std::string result = name + "(";
        for (size_t i = 0; i < parameters.size(); ++i) {
            if (i > 0) result += ",";
            result += parameters[i];
        }
        result += ")";
        return result;
    }
};

class Rule {
private:
    Predicate headPredicate;
    std::vector<Predicate> predicates;

public:
    Rule(const Predicate& head, const std::vector<Predicate>& preds) : headPredicate(head), predicates(preds) {}

    std::string toString() const {
        std::string result = headPredicate.toString() + " :- ";
        for (size_t i = 0; i < predicates.size(); ++i) {
            if (i > 0) result += ",";
            result += predicates[i].toString();
        }
        result += ".";
        return result;
    }
};

void parseFactList(Scanner& scanner, DatalogProgram& program);
void parseRuleList(Scanner& scanner, DatalogProgram& program);
void parseQueryList(Scanner& scanner, DatalogProgram& program);
void parseSchemeList(Scanner& scanner, DatalogProgram& program);
Predicate parseHeadPredicate(Scanner& scanner);
Predicate parsePredicate(Scanner& scanner);
std::vector<std::string> parseIdList(Scanner& scanner);

DatalogProgram parseDatalogProgram(Scanner& scanner) {
    DatalogProgram program;

    // Implement parsing of Datalog program here

    return program;
}

void parseFactList(Scanner& scanner, DatalogProgram& program) {
    // Implement parsing of fact list here
}

void parseRuleList(Scanner& scanner, DatalogProgram& program) {
    // Implement parsing of rule list here
}

void parseQueryList(Scanner& scanner, DatalogProgram& program) {
    // Implement parsing of query list here
}

void parseSchemeList(Scanner& scanner, DatalogProgram& program) {
    // Implement parsing of scheme list here
}

Predicate parseHeadPredicate(Scanner& scanner) {
    // Implement parsing of head predicate here
    return Predicate(""); // Dummy return
}

Predicate parsePredicate(Scanner& scanner) {
    // Implement parsing of predicate here
    return Predicate(""); // Dummy return
}

std::vector<std::string> parseIdList(Scanner& scanner) {
    // Implement parsing of ID list here
    return std::vector<std::string>(); // Dummy return
}

int main() {
    // Create a scanner and read input tokens
    Scanner scanner("...");

    // Parse Datalog program using tokens from the scanner
    DatalogProgram program = parseDatalogProgram(scanner);

    // Output results based on successful or unsuccessful parsing
    // Call program.toString() to print the Datalog program
    // Handle success or failure as described in the project description

    return 0;
}
