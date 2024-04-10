// ADDED IN PROJECT 3
#pragma once
#include "scheme.h"
#include "tuple.h"
#include <set>

class Relation {
public:
    Relation() {}
    Relation Select(int index, string value);
    Relation Select(int indexOne, int indexTwo);
    Relation Project(vector<int> indicies);
    Relation Rename(vector<string> names);
    Relation Join(Relation relationToJoin);
    bool Unite(Relation toUnite);
    bool isJoinable(Tuple Tuple1, Tuple Tuple2Add, Scheme Scheme1, Scheme Scheme2Add);
    void AddTuple(Tuple tuples); 
    void SetScheme(Scheme DemSchemes);
    void SetName(string DatName);
    Scheme GetScheme() {
        return TheScheme;
    }
    set<Tuple> GetTuple() {
        return DemTuples;
    }
    int GetTupleSize() {
        return DemTuples.size();
    }
    void ToString();
    set<Tuple> DemTuples;
    string name;
    Scheme TheScheme;
    bool BeenOutPutted;
};