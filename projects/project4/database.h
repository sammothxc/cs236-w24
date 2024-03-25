#pragma once
#include "relation.h"
#include <map>
#include <sstream>

class Database {
public:
    map<string, Relation> databaseseses;
    void AddRelation(string Name, Relation newRelation) {
        databaseseses.insert({Name, newRelation});
        return;
    }
    void AddTuplesToRelation(string Name, Tuple theTuple) {
        databaseseses.at(Name).AddTuple(theTuple);
        return;
    }
    string ToStirng() {
        stringstream stringy;
        map<string, Relation> ptr;
        for (auto i: databaseseses) {
            cout  <<  i.first << ": \n";
            i.second.ToString();
        }
        return stringy.str();
    }
    Relation GetRelation(string Name) {
        return databaseseses.at(Name);
    }
    int TupleQuantity() {
        int size = 0;
        for (auto i: databaseseses) {
            cout << "This is the Size: ";
            size += i.second.GetTupleSize();
            cout << size << endl;
        }
        return size;
    }
};