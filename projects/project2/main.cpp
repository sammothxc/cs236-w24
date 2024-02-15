#include "Scanner.h"
#include "Token.h"
#include "DatalogProgram.h"
#include "Predicate.h"
#include "Rule.h"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <set>
#include <algorithm>

using namespace std;

// Forward declaration
DatalogProgram parseDatalogProgram(Scanner& scanner);

void parseSchemeList(Scanner& scanner, DatalogProgram& program) {
    Token token = scanner.scanToken();
    while (token.getType() == ID) {
        Predicate scheme(token.typeName());
        
        // Parse scheme parameters
        token = scanner.scanToken(); // Expecting LEFT_PAREN
        if (token.getType() != LEFT_PAREN) {
            throw std::runtime_error("Expected LEFT_PAREN in scheme");
        }

        // Parse scheme parameters and add them to the scheme
        parseIdList(scanner, scheme);

        // Expect RIGHT_PAREN
        token = scanner.scanToken();
        if (token.getType() != RIGHT_PAREN) {
            throw std::runtime_error("Expected RIGHT_PAREN in scheme");
        }

        // Add the scheme to the Datalog program
        program.addScheme(scheme);

        // Expect COMMA or PERIOD
        token = scanner.scanToken();
        if (token.getType() == COMMA) {
            // Another scheme follows, continue parsing
            token = scanner.scanToken(); // Expecting another scheme or PERIOD
        } else if (token.getType() != PERIOD) {
            throw std::runtime_error("Expected COMMA or PERIOD after scheme");
        }
    }
}

// Function to parse a list of facts
void parseFactList(Scanner& scanner, DatalogProgram& program) {
    Token token = scanner.scanToken();
    while (token.getType() == ID) {
        Predicate fact(token.typeName()); // Corrected here

        // Expect LEFT_PAREN
        token = scanner.scanToken();
        if (token.getType() != LEFT_PAREN) {
            throw std::runtime_error("Expected LEFT_PAREN in fact");
        }

        // Parse fact parameters and add them to the fact
        parseStringList(scanner, fact);

        // Expect RIGHT_PAREN
        token = scanner.scanToken();
        if (token.getType() != RIGHT_PAREN) {
            throw std::runtime_error("Expected RIGHT_PAREN in fact");
        }

        // Expect PERIOD
        token = scanner.scanToken();
        if (token.getType() != PERIOD) {
            throw std::runtime_error("Expected PERIOD after fact");
        }

        // Add the fact to the Datalog program
        program.addFact(fact);

        // Get the next token
        token = scanner.scanToken();
    }
}


// Function to parse a list of rules
void parseRuleList(Scanner& scanner, DatalogProgram& program) {
    Token token = scanner.scanToken();
    while (token.getType() == ID) {
        Predicate headPredicate(token.typeName());

        // Expect LEFT_PAREN
        token = scanner.scanToken();
        if (token.getType() != LEFT_PAREN) {
            throw std::runtime_error("Expected LEFT_PAREN in rule head");
        }

        // Parse head predicate parameters and add them to the head predicate
        parseIdList(scanner, headPredicate);

        // Expect RIGHT_PAREN
        token = scanner.scanToken();
        if (token.getType() != RIGHT_PAREN) {
            throw std::runtime_error("Expected RIGHT_PAREN in rule head");
        }

        // Expect COLON_DASH
        token = scanner.scanToken();
        if (token.getType() != COLON_DASH) {
            throw std::runtime_error("Expected COLON_DASH in rule");
        }

        // Parse rule body predicates
        std::vector<Predicate> bodyPredicates;
        parsePredicateList(scanner, bodyPredicates);

        // Expect PERIOD
        token = scanner.scanToken();
        if (token.getType() != PERIOD) {
            throw std::runtime_error("Expected PERIOD after rule");
        }

        // Add the rule to the Datalog program
        program.addRule(Rule(headPredicate, bodyPredicates));

        // Get the next token
        token = scanner.scanToken();
    }
}

// Function to parse a list of queries
void parseQueryList(Scanner& scanner, DatalogProgram& program) {
    Token token = scanner.scanToken();
    while (token.getType() == ID) {
        Predicate query(token.typeName());

        // Expect LEFT_PAREN
        token = scanner.scanToken();
        if (token.getType() != LEFT_PAREN) {
            throw std::runtime_error("Expected LEFT_PAREN in query");
        }

        // Parse query parameters and add them to the query
        parseParameterList(scanner, query);

        // Expect RIGHT_PAREN
        token = scanner.scanToken();
        if (token.getType() != RIGHT_PAREN) {
            throw std::runtime_error("Expected RIGHT_PAREN in query");
        }

        // Expect Q_MARK
        token = scanner.scanToken();
        if (token.getType() != Q_MARK) {
            throw std::runtime_error("Expected Q_MARK after query");
        }

        // Add the query to the Datalog program
        program.addQuery(query);

        // Get the next token
        token = scanner.scanToken();
    }
}

// Function to parse a list of parameters
void parseParameterList(Scanner& scanner, Predicate& predicate) {
    Token token = scanner.scanToken();
    while (token.getType() == STRING || token.getType() == ID) {
        if (token.getType() == STRING) {
            predicate.addParameter(token.typeName());
        } else if (token.getType() == ID) {
            // Parameter is an ID, parse its ID list
            Predicate parameter(token.typeName());

            // Expect LEFT_PAREN
            token = scanner.scanToken();
            if (token.getType() != LEFT_PAREN) {
                throw std::runtime_error("Expected LEFT_PAREN in parameter");
            }

            // Parse parameter parameters and add them to the parameter
            parseIdList(scanner, parameter);

            // Expect RIGHT_PAREN
            token = scanner.scanToken();
            if (token.getType() != RIGHT_PAREN) {
                throw std::runtime_error("Expected RIGHT_PAREN in parameter");
            }

            // Add the parameter to the predicate
            predicate.addParameter(parameter.toString());
        }

        // Expect COMMA or end of parameter list
        token = scanner.scanToken();
        if (token.getType() == COMMA) {
            // Another parameter follows, continue parsing
            token = scanner.scanToken(); // Expecting another parameter or end of list
        } else if (token.getType() != RIGHT_PAREN) {
            throw std::runtime_error("Expected COMMA or RIGHT_PAREN in parameter list");
        }
    }
}

// Function to parse a list of predicates
void parsePredicateList(Scanner& scanner, std::vector<Predicate>& predicates) {
    Token token = scanner.scanToken();
    while (token.getType() == ID) {
        Predicate predicate(token.getTypeName());

        // Expect LEFT_PAREN
        token = scanner.scanToken();
        if (token.getType() != LEFT_PAREN) {
            throw std::runtime_error("Expected LEFT_PAREN in predicate");
        }

        // Parse predicate parameters and add them to the predicate
        parseParameterList(scanner, predicate);

        // Expect RIGHT_PAREN
        token = scanner.scanToken();
        if (token.getType() != RIGHT_PAREN) {
            throw std::runtime_error("Expected RIGHT_PAREN in predicate");
        }

        // Add the predicate to the list
        predicates.push_back(predicate);

        // Expect COMMA or end of predicate list
        token = scanner.scanToken();
        if (token.getType() == COMMA) {
            // Another predicate follows, continue parsing
            token = scanner.scanToken(); // Expecting another predicate or end of list
        } else if (token.getType() != PERIOD) {
            throw std::runtime_error("Expected COMMA or PERIOD in predicate list");
        }
    }
}

// Function to parse a list of strings
void parseStringList(Scanner& scanner, Predicate& predicate) {
    Token token = scanner.scanToken();
    while (token.getType() == STRING) {
        predicate.addParameter(token.getTypeName());

        // Expect COMMA or end of string list
        token = scanner.scanToken();
        if (token.getType() == COMMA) {
            // Another string follows, continue parsing
            token = scanner.scanToken(); // Expecting another string or end of list
        } else if (token.getType() != RIGHT_PAREN) {
            throw std::runtime_error("Expected COMMA or RIGHT_PAREN in string list");
        }
    }
}


int main() {
    // Create a scanner and read input tokens
    Scanner scanner(std::cin);

    // Parse Datalog program using tokens from the scanner
    DatalogProgram program = parseDatalogProgram(scanner);

    // Output results based on successful or unsuccessful parsing
    // Call program.toString() to print the Datalog program
    // Handle success or failure as described in the project description
    return 0;
}

// Function to parse a Datalog program
// Function to parse a Datalog program
DatalogProgram parseDatalogProgram(Scanner& scanner) {
    DatalogProgram program;

    try {
        // Expect SCHEMES
        Token token = scanner.scanToken();
        if (token.getType() != SCHEMES) {
            throw std::runtime_error("Expected SCHEMES");
        }

        // Expect COLON
        token = scanner.scanToken();
        if (token.getType() != COLON) {
            throw std::runtime_error("Expected COLON after SCHEMES");
        }

        // Parse schemes
        parseSchemeList(scanner, program);

        // Expect FACTS
        token = scanner.scanToken();
        if (token.getType() != FACTS) {
            throw std::runtime_error("Expected FACTS");
        }

        // Expect COLON
        token = scanner.scanToken();
        if (token.getType() != COLON) {
            throw std::runtime_error("Expected COLON after FACTS");
        }

        // Parse facts
        parseFactList(scanner, program);

        // Expect RULES
        token = scanner.scanToken();
        if (token.getType() != RULES) {
            throw std::runtime_error("Expected RULES");
        }

        // Expect COLON
        token = scanner.scanToken();
        if (token.getType() != COLON) {
            throw std::runtime_error("Expected COLON after RULES");
        }

        // Parse rules
        parseRuleList(scanner, program);

        // Expect QUERIES
        token = scanner.scanToken();
        if (token.getType() != QUERIES) {
            throw std::runtime_error("Expected QUERIES");
        }

        // Expect COLON
        token = scanner.scanToken();
        if (token.getType() != COLON) {
            throw std::runtime_error("Expected COLON after QUERIES");
        }

        // Parse queries
        parseQueryList(scanner, program);

        // Expect EOF
        token = scanner.scanToken();
        if (token.getType() != EOF_TOKEN) {
            throw std::runtime_error("Expected EOF");
        }

    } catch (const std::exception& e) {
        std::cerr << "Failure! " << e.what() << std::endl;
        return DatalogProgram(); // Return empty program on failure
    }

    std::cout << "Success!" << std::endl;
    return program;
}