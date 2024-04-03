int main() {

  // predicate names for fake rules
  // first is name for head predicate
  // second is names for body predicates
  pair<string,vector<string>> ruleNames[] = {
    { "A", { "B" } },
    { "B", { "B", "A" } },
  };

  vector<Rule> rules;

  for (auto& rulePair : ruleNames) {
    string headName = rulePair.first;
    Rule rule = Rule(Predicate(headName));
    vector<string> bodyNames = rulePair.second;
    for (auto& bodyName : bodyNames)
      rule.addBodyPredicate(Predicate(bodyName));
    rules.push_back(rule);
  }

  Graph graph = Interpreter::makeGraph(rules);
  cout << graph.toString();

}