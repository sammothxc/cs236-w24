#pragma once
#include "Predicate.h"

class Rule {
    public:
        void SetHeadPredicate(Predicate head);
        void SetRule(vector<Predicate> Toke);
        string ToString();
    private:
        Predicate Head;
        vector<Predicate> RuleList;
};
