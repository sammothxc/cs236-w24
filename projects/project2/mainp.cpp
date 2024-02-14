#include <iostream>
#include <vector>
#include <set>
#include <stdexcept>

// Forward declaration of classes
class Parameter;
class Predicate;
class Rule;
class DatalogProgram;

// Class representing a Parameter in a Predicate
class Parameter {
public:
    std::string value;
    bool isConstant;

    Parameter(const std::string& val, bool constant) : value(val), isConstant(constant) {}

    std::string toString() const {
        return value;
    }
};

// Class representing a Predicate in a Datalog program
class Predicate {
public:
    std::string name;
    std::vector<Parameter> parameters;

    Predicate(const std::string& n) : name(n) {}

    std::string toString() const {
        std::string result = name + "(";
        for (size_t i = 0; i < parameters.size(); ++i) {
            result += parameters[i].toString();
            if (i != parameters.size() - 1)
                result += ",";
        }
        result += ")";
        return result;
    }
};

// Class representing a Rule in a Datalog program
class Rule {
public:
    Predicate headPredicate;
    std::vector<Predicate> bodyPredicates;

    Rule(const Predicate& head) : headPredicate(head) {}

    std::string toString() const {
        std::string result = headPredicate.toString() + " :- ";
        for (size_t i = 0; i < bodyPredicates.size(); ++i) {
            result += bodyPredicates[i].toString();
            if (i != bodyPredicates.size() - 1)
                result += ",";
        }
        return result;
    }
};

// Class representing the entire Datalog program
class DatalogProgram {
public:
    std::vector<Predicate> schemes;
    std::vector<Predicate> facts;
    std::vector<Rule> rules;
    std::vector<Predicate> queries;
    std::set<std::string> domain;

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

// Token types
enum class TokenType { SCHEMES, FACTS, RULES, QUERIES, COLON, COMMA, PERIOD, Q_MARK, LEFT_PAREN, RIGHT_PAREN, ID, STRING, EOF_TYPE };

// Scanner class for tokenizing input
class Scanner {
public:
    Scanner(std::istream& input) : input(input), lineNumber(1) {}

    TokenType getNextToken() {
        char ch;
        while (input.get(ch)) {
            switch (ch) {
                case ' ':
                case '\t':
                case '\r':
                    continue; // Ignore whitespace
                case '\n':
                    ++lineNumber;
                    continue; // Ignore newline
                case ':':
                    return TokenType::COLON;
                case ',':
                    return TokenType::COMMA;
                case '.':
                    return TokenType::PERIOD;
                case '?':
                    return TokenType::Q_MARK;
                case '(':
                    return TokenType::LEFT_PAREN;
                case ')':
                    return TokenType::RIGHT_PAREN;
                case '\'':
                    return readString();
                default:
                    if (isalpha(ch)) {
                        input.putback(ch); // Put back the first character of the identifier
                        return readIdentifier();
                    } else {
                        throw std::runtime_error("Invalid character '" + std::string(1, ch) + "' on line " + std::to_string(lineNumber));
                    }
            }
        }
        return TokenType::EOF_TYPE;
    }

    std::string getTokenValue() const {
        return tokenValue;
    }

    int getLineNumber() const {
        return lineNumber;
    }

private:
    std::istream& input;
    int lineNumber;
    std::string tokenValue;

    TokenType readString() {
        tokenValue.clear();
        char ch;
        while (input.get(ch)) {
            if (ch == '\'') {
                return TokenType::STRING;
            } else {
                tokenValue += ch;
            }
        }
        throw std::runtime_error("Unterminated string starting on line " + std::to_string(lineNumber));
    }

    TokenType readIdentifier() {
        tokenValue.clear();
        char ch;
        while (input.get(ch)) {
            if (isalnum(ch) || ch == '_') {
                tokenValue += ch;
            } else {
                input.putback(ch); // Put back the non-alphanumeric character
                return TokenType::ID;
            }
        }
        return TokenType::EOF_TYPE;
    }
};

// Recursive descent parser for Datalog programs
class Parser {
public:
    Parser(Scanner& scanner) : scanner(scanner) {}

    DatalogProgram parse() {
        DatalogProgram program;
        parseDatalogProgram(program);
        return program;
    }

private:
    Scanner& scanner;

    void parseDatalogProgram(DatalogProgram& program) {
        expect(TokenType::SCHEMES);
        expect(TokenType::COLON);
        parseSchemeList(program);
        expect(TokenType::FACTS);
        expect(TokenType::COLON);
        parseFactList(program);
        expect(TokenType::RULES);
        expect(TokenType::COLON);
        parseRuleList(program);
        expect(TokenType::QUERIES);
        expect(TokenType::COLON);
        parseQueryList(program);
        expect(TokenType::EOF_TYPE);
    }

    void parseSchemeList(DatalogProgram& program) {
        while (scanner.getNextToken() == TokenType::ID) {
            program.schemes.push_back(parseScheme());
        }
    }

    Predicate parseScheme() {
        Predicate scheme = parsePredicate();
        expect(TokenType::PERIOD);
        return scheme;
    }

    void parseFactList(DatalogProgram& program) {
        while (scanner.getNextToken() == TokenType::ID) {
            program.facts.push_back(parseFact());
        }
    }

    Predicate parseFact() {
        Predicate fact = parsePredicate();
        expect(TokenType::PERIOD);
        return fact;
    }

    void parseRuleList(DatalogProgram& program) {
        while (scanner.getNextToken() == TokenType::ID) {
            program.rules.push_back(parseRule());
        }
    }

    Rule parseRule() {
        Predicate headPredicate = parsePredicate();
        expect(TokenType::COLON);
        expect(TokenType::DASH);
        std::vector<Predicate> bodyPredicates;
        do {
            bodyPredicates.push_back(parsePredicate());
        } while (scanner.getNextToken() == TokenType::COMMA);
        expect(TokenType::PERIOD);
        return Rule(headPredicate, bodyPredicates);
    }

    void parseQueryList(DatalogProgram& program) {
        while (scanner.getNextToken() == TokenType::ID) {
            program.queries.push_back(parsePredicate());
            expect(TokenType::Q_MARK);
        }
    }

    Predicate parsePredicate() {
        std::string name = scanner.getTokenValue();
        expect(TokenType::ID);
        expect(TokenType::LEFT_PAREN);
        std::vector<Parameter> parameters;
        if (scanner.getNextToken() == TokenType::ID || scanner.getNextToken() == TokenType::STRING) {
            parameters.push_back(parseParameter());
            while (scanner.getNextToken() == TokenType::COMMA) {
                parameters.push_back(parseParameter());
            }
        }
        expect(TokenType::RIGHT_PAREN);
        return Predicate(name, parameters);
    }

    Parameter parseParameter() {
        if (scanner.getNextToken() == TokenType::ID) {
            return Parameter(scanner.getTokenValue(), false);
        } else if (scanner.getNextToken() == TokenType::STRING) {
            return Parameter(scanner.getTokenValue(), true);
        } else {
            throw std::runtime_error("Expected parameter on line " + std::to_string(scanner.getLineNumber()));
        }
    }

    void expect(TokenType expected) {
        TokenType actual = scanner.getNextToken();
        if (actual != expected) {
            throw std::runtime_error("Expected token type " + tokenTypeToString(expected) + " but got " + tokenTypeToString(actual) + " on line " + std::to_string(scanner.getLineNumber()));
        }
    }

    std::string tokenTypeToString(TokenType type) {
        switch (type) {
            case TokenType::SCHEMES: return "SCHEMES";
            case TokenType::FACTS: return "FACTS";
            case TokenType::RULES: return "RULES";
            case TokenType::QUERIES: return "QUERIES";
            case TokenType::COLON: return "COLON";
            case TokenType::COMMA: return "COMMA";
            case TokenType::PERIOD: return "PERIOD";
            case TokenType::Q_MARK: return "Q_MARK";
            case TokenType::LEFT_PAREN: return "LEFT_PAREN";
            case TokenType::RIGHT_PAREN: return "RIGHT_PAREN";
            case TokenType::ID: return "ID";
            case TokenType::STRING: return "STRING";
            case TokenType::EOF_TYPE: return "EOF_TYPE";
        }
        return "";
    }
};

int main() {
    // Example usage
    // Create a scanner from input stream
    Scanner scanner(std::cin);
    // Create a parser with the scanner
    Parser parser(scanner);
    // Parse the Datalog program
    DatalogProgram program = parser.parse();
    // Output the parsed program
    std::cout << "Success!\n" << program.toString() << std::endl;
    return 0;
}
