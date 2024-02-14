#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <sstream>
#include <cctype>
#include <stdexcept>

// Token types
enum TokenType {
    SCHEMES, FACTS, RULES, QUERIES,
    ID, STRING, COLON, COMMA, Q_MARK, LEFT_PAREN, RIGHT_PAREN, COLON_DASH,
    PERIOD, EOF_TOKEN, UNDEFINED
};

// Token structure
struct Token {
    TokenType type;
    std::string value;
    int lineNumber;
    
    // Constructor
    Token() : type(UNDEFINED), value(""), lineNumber(0) {}
    Token(TokenType type, const std::string& value, int lineNumber)
        : type(type), value(value), lineNumber(lineNumber) {}
};

// Scanner class
class Scanner {
public:
    Scanner(std::istream& input) : input(input), lineNumber(1) {}

    Token getNextToken() {
        char ch;
        do {
            ch = input.get();
            if (ch == '\n') lineNumber++;
        } while (isspace(ch));
        
        if (input.eof()) return Token(EOF_TOKEN, "", lineNumber);
        
        switch (ch) {
            case ',': return Token(COMMA, ",", lineNumber);
            case '.': return Token(PERIOD, ".", lineNumber);
            case '?': return Token(Q_MARK, "?", lineNumber);
            case ':':
                if (input.peek() == '-') {
                    input.get(); // consume '-'
                    return Token(COLON_DASH, ":-", lineNumber);
                } else {
                    return Token(COLON, ":", lineNumber);
                }
            case '(':
                return Token(LEFT_PAREN, "(", lineNumber);
            case ')':
                return Token(RIGHT_PAREN, ")", lineNumber);
            case '\'':
                return scanString();
            default:
                if (isalpha(ch)) {
                    return scanId(ch);
                } else {
                    return Token(UNDEFINED, std::string(1, ch), lineNumber);
                }
        }
    }

    Token getToken() const {
        return currentToken;
    }

private:
    std::istream& input;
    int lineNumber;
    Token currentToken;

    Token scanString() {
        std::string value;
        char ch;
        value += '\'';
        while ((ch = input.get()) != '\'') {
            if (ch == '\n' || ch == EOF) {
                throw std::runtime_error("Unterminated string literal");
            }
            value += ch;
        }
        value += '\'';
        return Token(STRING, value, lineNumber);
    }

    Token scanId(char ch) {
        std::string value;
        value += ch;
        while (isalnum(input.peek())) {
            value += input.get();
        }
        return Token(getIDType(value), value, lineNumber);
    }

    TokenType getIDType(const std::string& value) {
        if (value == "Schemes") return SCHEMES;
        if (value == "Facts") return FACTS;
        if (value == "Rules") return RULES;
        if (value == "Queries") return QUERIES;
        return ID;
    }
};

// Parameter class
class Parameter {
public:
    Parameter(const std::string& value) : value(value) {}

    std::string toString() const {
        return value;
    }

private:
    std::string value;
};

// Predicate class
class Predicate {
public:
    Predicate(const std::string& name) : name(name) {}

    void addParameter(const Parameter& param) {
        parameters.push_back(param);
    }

    std::string toString() const {
        std::string result = name + "(";
        for (size_t i = 0; i < parameters.size(); ++i) {
            if (i > 0) result += ",";
            result += parameters[i].toString();
        }
        result += ")";
        return result;
    }

private:
    std::string name;
    std::vector<Parameter> parameters;
};

// Rule class
class Rule {
public:
    Rule() : headPredicate(""), predicates({}) {}
    Rule(const Predicate& head, const std::vector<Predicate>& preds) : headPredicate(head), predicates(preds) {}
    
    void setHeadPredicate(const Predicate& head) {
        headPredicate = head;
    }

    void addPredicate(const Predicate& pred) {
        predicates.push_back(pred);
    }

    std::string toString() const {
        std::string result = headPredicate.toString() + " :- ";
        for (size_t i = 0; i < predicates.size(); ++i) {
            if (i > 0) result += ",";
            result += predicates[i].toString();
        }
        result += ".";
        return result;
    }

private:
    Predicate headPredicate;
    std::vector<Predicate> predicates;
};

// DatalogProgram class
class DatalogProgram {
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

    void addToDomain(const std::string& value) {
        domain.insert(value);
    }

private:
    std::vector<Predicate> schemes;
    std::vector<Predicate> facts;
    std::vector<Rule> rules;
    std::vector<Predicate> queries;
    std::set<std::string> domain;
};

// Function prototypes
DatalogProgram datalogProgram(Scanner& scanner);
void schemes(DatalogProgram& program, Scanner& scanner);
void facts(DatalogProgram& program, Scanner& scanner);
void rules(DatalogProgram& program, Scanner& scanner);
void queries(DatalogProgram& program, Scanner& scanner);
Predicate scheme(Scanner& scanner);
Predicate fact(Scanner& scanner);
Rule rule(Scanner& scanner);
Predicate query(Scanner& scanner);
void idList(Predicate& predicate, Scanner& scanner);
void parameterList(Predicate& predicate, Scanner& scanner);
void stringList(Predicate& predicate, Scanner& scanner);
Predicate predicate(Scanner& scanner);

// Parsing functions
DatalogProgram datalogProgram(Scanner& scanner) {
    DatalogProgram program;
    try {
        schemes(program, scanner);
        facts(program, scanner);
        rules(program, scanner);
        queries(program, scanner);
        if (scanner.getToken().type != EOF_TOKEN) {
            throw std::runtime_error("Unexpected token at end of input");
        }
    } catch (const std::exception& e) {
        std::cout << "Failure!\n  (" << scanner.getToken().type << ",\"" 
                  << scanner.getToken().value << "\"," << scanner.getToken().lineNumber << ")\n";
        return DatalogProgram(); // Return empty program on failure
    }
    std::cout << "Success!\n";
    return program;
}

void schemes(DatalogProgram& program, Scanner& scanner) {
    if (scanner.getToken().type != SCHEMES) {
        throw std::runtime_error("Expected SCHEMES token");
    }
    scanner.getNextToken(); // Consume SCHEMES token
    if (scanner.getToken().type != COLON) {
        throw std::runtime_error("Expected COLON token after SCHEMES");
    }
    scanner.getNextToken(); // Consume COLON token
    while (scanner.getToken().type == ID) {
        program.addScheme(scheme(scanner));
    }
}

void facts(DatalogProgram& program, Scanner& scanner) {
    if (scanner.getToken().type != FACTS) {
        throw std::runtime_error("Expected FACTS token");
    }
    scanner.getNextToken(); // Consume FACTS token
    if (scanner.getToken().type != COLON) {
        throw std::runtime_error("Expected COLON token after FACTS");
    }
    scanner.getNextToken(); // Consume COLON token
    while (scanner.getToken().type == ID) {
        program.addFact(fact(scanner));
    }
}

void rules(DatalogProgram& program, Scanner& scanner) {
    if (scanner.getToken().type != RULES) {
        throw std::runtime_error("Expected RULES token");
    }
    scanner.getNextToken(); // Consume RULES token
    if (scanner.getToken().type != COLON) {
        throw std::runtime_error("Expected COLON token after RULES");
    }
    scanner.getNextToken(); // Consume COLON token
    while (scanner.getToken().type == ID) {
        program.addRule(rule(scanner));
    }
}

void queries(DatalogProgram& program, Scanner& scanner) {
    if (scanner.getToken().type != QUERIES) {
        throw std::runtime_error("Expected QUERIES token");
    }
    scanner.getNextToken(); // Consume QUERIES token
    if (scanner.getToken().type != COLON) {
        throw std::runtime_error("Expected COLON token after QUERIES");
    }
    scanner.getNextToken(); // Consume COLON token
    while (scanner.getToken().type == ID) {
        program.addQuery(query(scanner));
    }
}

Predicate scheme(Scanner& scanner) {
    if (scanner.getToken().type != ID) {
        throw std::runtime_error("Expected ID token for scheme");
    }
    Predicate pred(scanner.getToken().value);
    scanner.getNextToken(); // Consume ID token
    if (scanner.getToken().type != LEFT_PAREN) {
        throw std::runtime_error("Expected LEFT_PAREN token after scheme name");
    }
    scanner.getNextToken(); // Consume LEFT_PAREN token
    if (scanner.getToken().type != ID) {
        throw std::runtime_error("Expected ID token after LEFT_PAREN in scheme");
    }
    idList(pred, scanner);
    if (scanner.getToken().type != RIGHT_PAREN) {
        throw std::runtime_error("Expected RIGHT_PAREN token at end of scheme");
    }
    scanner.getNextToken(); // Consume RIGHT_PAREN token
    return pred;
}

Predicate fact(Scanner& scanner) {
    if (scanner.getToken().type != ID) {
        throw std::runtime_error("Expected ID token for fact");
    }
    Predicate pred(scanner.getToken().value);
    scanner.getNextToken(); // Consume ID token
    if (scanner.getToken().type != LEFT_PAREN) {
        throw std::runtime_error("Expected LEFT_PAREN token after fact name");
    }
    scanner.getNextToken(); // Consume LEFT_PAREN token
    if (scanner.getToken().type != STRING) {
        throw std::runtime_error("Expected STRING token after LEFT_PAREN in fact");
    }
    stringList(pred, scanner);
    if (scanner.getToken().type != RIGHT_PAREN) {
        throw std::runtime_error("Expected RIGHT_PAREN token at end of fact");
    }
    scanner.getNextToken(); // Consume RIGHT_PAREN token
    if (scanner.getToken().type != PERIOD) {
        throw std::runtime_error("Expected PERIOD token at end of fact");
    }
    scanner.getNextToken(); // Consume PERIOD token
    return pred;
}

Rule rule(Scanner& scanner) {
    Rule rule;
    rule.setHeadPredicate(scheme(scanner));
    if (scanner.getToken().type != COLON_DASH) {
        throw std::runtime_error("Expected COLON_DASH token in rule");
    }
    scanner.getNextToken(); // Consume COLON_DASH token
    rule.addPredicate(predicate(scanner));
    while (scanner.getToken().type == COMMA) {
        scanner.getNextToken(); // Consume COMMA token
        rule.addPredicate(predicate(scanner));
    }
    if (scanner.getToken().type != PERIOD) {
        throw std::runtime_error("Expected PERIOD token at end of rule");
    }
    scanner.getNextToken(); // Consume PERIOD token
    return rule;
}

Predicate query(Scanner& scanner) {
    Predicate pred = predicate(scanner);
    if (scanner.getToken().type != Q_MARK) {
        throw std::runtime_error("Expected Q_MARK token at end of query");
    }
    scanner.getNextToken(); // Consume Q_MARK token
    return pred;
}

void idList(Predicate& predicate, Scanner& scanner) {
    if (scanner.getToken().type != ID) {
        throw std::runtime_error("Expected ID token in idList");
    }
    predicate.addParameter(Parameter(scanner.getToken().value));
    scanner.getNextToken(); // Consume ID token
    while (scanner.getToken().type == COMMA) {
        scanner.getNextToken(); // Consume COMMA token
        if (scanner.getToken().type != ID) {
            throw std::runtime_error("Expected ID token after COMMA in idList");
        }
        predicate.addParameter(Parameter(scanner.getToken().value));
        scanner.getNextToken(); // Consume ID token
    }
}

void parameterList(Predicate& predicate, Scanner& scanner) {
    if (scanner.getToken().type != STRING && scanner.getToken().type != ID) {
        throw std::runtime_error("Expected STRING or ID token in parameterList");
    }
    predicate.addParameter(Parameter(scanner.getToken().value));
    scanner.getNextToken(); // Consume STRING or ID token
    while (scanner.getToken().type == COMMA) {
        scanner.getNextToken(); // Consume COMMA token
        if (scanner.getToken().type != STRING && scanner.getToken().type != ID) {
            throw std::runtime_error("Expected STRING or ID token after COMMA in parameterList");
        }
        predicate.addParameter(Parameter(scanner.getToken().value));
        scanner.getNextToken(); // Consume STRING or ID token
    }
}

void stringList(Predicate& predicate, Scanner& scanner) {
    if (scanner.getToken().type != STRING) {
        throw std::runtime_error("Expected STRING token in stringList");
    }
    predicate.addParameter(Parameter(scanner.getToken().value));
    scanner.getNextToken(); // Consume STRING token
    while (scanner.getToken().type == COMMA) {
        scanner.getNextToken(); // Consume COMMA token
        if (scanner.getToken().type != STRING) {
            throw std::runtime_error("Expected STRING token after COMMA in stringList");
        }
        predicate.addParameter(Parameter(scanner.getToken().value));
        scanner.getNextToken(); // Consume STRING token
    }
}

// Main function
int main() {
    Scanner scanner(std::cin);
    DatalogProgram program = datalogProgram(scanner);
    std::cout << program.toString() << std::endl;
    return 0;
}
