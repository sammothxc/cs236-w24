// ADDED IN PROJECT 3
#pragma once
#include "database.h"
#include "datalog.h"
#include "Graph.h"

class Interpreter {
    private:
        Database theDatabase;
        Datalog theDatalog;
    public:
        void SetDatalog(Datalog theInformation);

        void SchemesToDatabase(vector<Predicate> Schemes);
        void FactsToDatabase(vector<Predicate> Facts);
        void RelationQueries(vector<Predicate> Queries);
        void RelationRules(vector<Rule> Rules, bool dependent);
        Relation EvalutatePredicate(Predicate Predicates);
        void RuleOptimization(vector<Rule>Rules);
        string ToString();
};