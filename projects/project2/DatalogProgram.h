#pragma once
#include <string>
#include <vector>
#include "Token.h"
#include "Predicate.h"
#include "Rule.h"

// Class representing a list of schemes
class SchemeList {
private:
    std::vector<Predicate> schemes;

public:
    // Methods to add, access, and manipulate schemes
};

// Class representing a list of facts
class FactList {
private:
    std::vector<Predicate> facts;

public:
    // Methods to add, access, and manipulate facts
};

// Class representing a list of rules
class RuleList {
private:
    std::vector<Rule> rules;

public:
    // Methods to add, access, and manipulate rules
};

// Class representing a list of queries
class QueryList {
private:
    std::vector<Predicate> queries;

public:
    // Methods to add, access, and manipulate queries
};

class DatalogProgram {
private:
    SchemeList schemes;
    FactList facts;
    RuleList rules;
    QueryList queries;
    std::set<std::string> domain; // Set to store unique strings

public:
    // Constructor, Access Methods, Update Methods, toString, etc.
};