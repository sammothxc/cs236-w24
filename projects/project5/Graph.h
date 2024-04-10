// ADDED IN PROJECT 5
#pragma once
#include <map>
#include "Node.h"
#include <stack>
#include <iostream>
#include <vector>
using namespace std;

class Graph {
    public:
    map<int, Node> nodeList;
    stack<int> topologicalSearch;
    vector<set<int>> scc;
    set<int>local;
    void dfs(Node theNode) {
        if (theNode.isVisited == true) {
            return;
        }
        else {
            nodeList.at(theNode.id).isVisited = true;
            for (auto i: theNode.adjacencies) {
                if (nodeList[i].isVisited == false) {
                    dfs(nodeList[i]);
                }
            }
            topologicalSearch.push(theNode.id);
            return;
        }
    }
    void dfsForest() {
        for (auto i: nodeList) {
            if(i.second.isVisited == false) {
                dfs(i.second);
            }
            else {
            }
        }
        return;
    }
    void dfsForestForward() {
        while(topologicalSearch.size() != 0){
            if (!nodeList[topologicalSearch.top()].isVisited) {
                dfsSCC(nodeList[topologicalSearch.top()]);
            }
            else {
                if (local.size() != 0) {
                    scc.push_back(local);
                    local.clear();
                }
                topologicalSearch.pop();
            }
        }
        return;
    }
    void dfsSCC(Node theNode) {
        if (theNode.isVisited == true) {
            
            return;
        }
        else {
            nodeList.at(theNode.id).isVisited = true;
            local.insert(theNode.id);
            for (auto i: theNode.adjacencies) {
                if (nodeList[i].isVisited == false) {
                    dfsSCC(nodeList[i]);
                }
            }
            return;
        }
    }
    void addEdge(int from, int to, bool dependent) {
        nodeList[from].adjacencies.insert(to);
        nodeList[from].isSelfDependent = dependent;
        return;
    }
    void ToString() {
        cout << "Dependency Graph" << endl;
        for(auto i:nodeList) {
            cout << "R" << i.first << ":";
            int x = i.second.adjacencies.size();
            int y = 0;
            for (auto j: i.second.adjacencies) {
                if (y != x - 1) {
                    cout <<"R" << j <<",";
                }
                else {
                     cout <<"R" << j;
                }
                y++;
            }
            cout << endl;
        }
        return;
    }
};
