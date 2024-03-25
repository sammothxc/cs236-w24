// ADDED IN PROJECT 2
#include "parser.h"

Parser::Parser() {}
Parser::~Parser(){}
Datalog Parser::GetDatalog() {
    return Everything;
}
bool Parser::DatalogProgram(queue<Token> Tokens) {
    Parse(Tokens);
    return fail;
}
string Parser::ToString() {
    if (fail != true) {
        string thing;
        cout << Everything.ToString();
        return thing;
    }
    else {
        string stringy;
        return stringy;
    }
}
Datalog Parser::Parse(queue<Token> Tokens) {
    TokenQueue = Tokens;
    vector<Predicate> Schemes;
    ParseCheck(SCHEMES);
    ParseCheck(COLON);
    Schemes.push_back(ParseScheme());
    Schemes = ParseSchemeList(Schemes);
    if (fail == false) {
        Everything.SetSchemes(Schemes);
    }
    else {
        return Everything;
    }
    if(fail == false) {
        ParseCheck(FACTS);
        ParseCheck(COLON);
        vector<Predicate> parsedFacts; 
        parsedFacts = ParseFactList(parsedFacts);
        if (fail == false) {
            Everything.SetFacts(parsedFacts);
            Everything.SetDomain(Domain);
        }
        else {
            return Everything;
    }
    }
    else {
        return Everything;
    }
    if(fail == false) {
        vector<Rule> Rules;
        ParseCheck(RULES);
        ParseCheck(COLON);
        Rules = ParseRuleList(Rules);
        if (fail == false) {
            Everything.SetRules(Rules);
        }
        else {
            return Everything;
        }
    }
    else {
        return Everything;
    }
    if (fail == false) {
        vector<Predicate> Queries;
        ParseCheck(QUERIES);
        ParseCheck(COLON);
        Queries.push_back(ParseQuery());
        Queries = ParseQueryList(Queries);
        if ( fail == false) {
            Everything.SetQueries(Queries);
        }
        else {
            return Everything;
    }
    }
    else {
        return Everything;
    }
    if (fail == false) {
        if (nextToken.GetType() == EOFa) {
            return Everything;
        }
        else {
            while (nextToken.GetType() != EOFa) {
                thisToken = TokenQueue.front();
                TokenQueue.pop();
                nextToken = TokenQueue.front();
            }
            PrintFail(thisToken.GetLineNumber());
            return Everything;
        }
    }
    else {
        return Everything;
    }
}
Predicate Parser::ParseQuery() {
    Predicate Query;
    if (fail == false) {
        Query = ParsePredicate();
        ParseCheck(Q_MARK);
        return Query;
    }
    else {
        return Query;
    }
}
vector<Predicate> Parser::ParseQueryList(vector<Predicate> Queries) {
    if (fail == false) {
        if (nextToken.GetType() != ID) {
            return Queries;
        }
        else {
            Queries.push_back(ParseQuery());
            Queries = ParseQueryList(Queries);
            return Queries;
        }
    }
    else {
        return Queries;
    }
}
vector<Rule> Parser::ParseRuleList(vector<Rule> RuleList) {
    if(fail == false) {
        if (nextToken.GetType() == QUERIES) {
            return RuleList;
        }
        else {
            RuleList.push_back(ParseRules());
            RuleList = ParseRuleList(RuleList);
            return RuleList;
        }
    }
    else {
        return RuleList;
    }
}
Rule Parser::ParseRules() {
    Rule theRule;
    vector<Predicate> listOfRules;
    Predicate rule;
    if (fail == false) {
        theRule.SetHeadPredicate(ParseHeadPredicate());
        ParseCheck(COLON_DASH);
        rule = ParsePredicate();
        listOfRules.push_back(rule);
        listOfRules = ParsePredicateList(listOfRules);
        ParseCheck(PERIOD);
        theRule.SetRule(listOfRules);
        return theRule;
    }
    else {
        return theRule;
    }
}
vector<Predicate> Parser::ParseFactList(vector<Predicate> Facts) {
    Predicate newFact;
    if (fail == false) {
        if (nextToken.GetType() == RULES) {
            return Facts; 
        }
        else {
            newFact = ParseFact();
            Facts.push_back(newFact);
            vector<Parameter> ParamList;
            ParamList = newFact.ReturnVector();
            for (unsigned int i = 0; i < ParamList.size(); i++) {
                Domain.insert(ParamList.at(i).ToString());
            }
            Facts = ParseFactList(Facts);
            return Facts;
        }
    }
    else {
        return Facts;
    }
}
Predicate Parser::ParseFact() {
    Predicate Facts;
    if (fail == false) {
        paramList.clear();
        ParseCheck(ID);
        Facts.SetName(thisToken.GetValue());
        ParseCheck(LEFT_PAREN);
        ParseCheck(STRING);
        PushOnList(thisToken.GetValue(), true);
        Domain.insert(thisToken.GetValue());
        ParseStringList();
        ParseCheck(RIGHT_PAREN);
        ParseCheck(PERIOD);
        for (unsigned int i = 0; i < paramList.size(); i++) {
            Facts.PushPredicate(paramList.at(i));
        }
        return Facts;
    }
    else {
        return Facts;
    } 
}
void Parser::ParseStringList(){
    if (fail == false) {
        if (nextToken.GetType() == COMMA) {
            ParseCheck(COMMA);
            ParseCheck(STRING);
            PushOnList(thisToken.GetValue(), true);
            ParseStringList();
        }
        else {
            return;
        }
    }
    else {
        return;
    }
}
Predicate Parser::ParseScheme() {
    Predicate Schemes;
    if (fail == false) {
        paramList.clear();
        ParseCheck(ID);
        Schemes.SetName(thisToken.GetValue());
        ParseCheck(LEFT_PAREN);
        ParseCheck(ID);
        PushOnList(thisToken.GetValue(), false);
        ParseIDList();
        ParseCheck(RIGHT_PAREN);
        for (unsigned int i = 0; i < paramList.size(); i++) {
            Schemes.PushPredicate(paramList.at(i));
        }
        return Schemes;
    }
    else {
        return Schemes;
    }
}
vector<Predicate> Parser::ParseSchemeList(vector<Predicate> Schemes) {
    Predicate newScheme; 
    if (fail == false) {
        if (nextToken.GetType() == FACTS) {
            return Schemes;
        }
        else {
            newScheme = ParseScheme();
            Schemes.push_back(newScheme);
            Schemes = ParseSchemeList(Schemes);
            return Schemes;
        }
    }
    else {
        return Schemes;
    }
}
void Parser::ParseIDList() {
    if (fail == false) {
        if (nextToken.GetType() == COMMA) {
            ParseCheck(COMMA);
            ParseCheck(ID);
            PushOnList(thisToken.GetValue(), false);
            ParseIDList();
        }
        else {
            return;
        }
    }
    else {
        return;
    }
}
bool Parser::CheckNext(TokenType type) {
    if (fail == false) {
        if (nextToken.GetType() == type) {
            return true;
        }
        else {
            PrintFail(nextToken.GetLineNumber());
            return false;
        }
    }
    else {
        return false;
    }
}
bool Parser::ParseCheck(TokenType type) {
    if (fail == false) {
        if (nextToken.GetType() != EOFa) {
            thisToken = TokenQueue.front();
            TokenQueue.pop();
            nextToken = TokenQueue.front();
            if (thisToken.GetType() == type) {
                return true; 
            }
            else {
                PrintFail(thisToken.GetLineNumber());
                return false;
            }
        }
        else {
            thisToken = TokenQueue.front();
            return true;
        }
    }
    else {
        if (nextToken.GetType() != EOFa) {
            thisToken = TokenQueue.front();
            TokenQueue.pop();
            nextToken = TokenQueue.front();
        }
        else {
            thisToken = TokenQueue.front();
        }
        return false;
    }
}
void Parser::PrintFail(int i) {
    cout << "Failure!" << "\n  (" << thisToken.TypeString() << ",\"" <<thisToken.GetValue() <<"\"," << i << ")" << endl;
    fail = true;
    return;
}
Predicate Parser::ParseHeadPredicate() {
    Predicate headPredicate;
    if (fail == false) {
        paramList.clear();
        ParseCheck(ID);
        headPredicate.SetName(thisToken.GetValue());
        ParseCheck(LEFT_PAREN);
        ParseCheck(ID);
        PushOnList(thisToken.GetValue(), false);
        ParseIDList();
        ParseCheck(RIGHT_PAREN);
        for (unsigned int i = 0; i < paramList.size(); i++) {
            headPredicate.PushPredicate(paramList.at(i));
        }
        return headPredicate;
    }
    else {
        return headPredicate;
    }
}
vector<Predicate> Parser::ParsePredicateList(vector<Predicate> list) {
    if (fail == false) {
        if (nextToken.GetType() == COMMA) {
            ParseCheck(COMMA);
            list.push_back(ParsePredicate());
            list = ParsePredicateList(list);
            return list;
        }
        else {
            return list;
        }
    }
    else {
        return list;
    }
}
Predicate Parser::ParsePredicate(){
    Predicate Predicates;
    Parameter newParam;
    if (fail == false) {
        paramList.clear();
        ParseCheck(ID);
        Predicates.SetName(thisToken.GetValue());
        ParseCheck(LEFT_PAREN);
        newParam = ParseParameter();
        PushOnList(newParam.ToString(), newParam.IsString());
        ParseParameterList();
        ParseCheck(RIGHT_PAREN);
        for (unsigned int i = 0; i < paramList.size(); i++) {
            Predicates.PushPredicate(paramList.at(i));
        }
        return Predicates;
    }
    else {
        return Predicates;
    }
}
void Parser::PushOnList(string token, bool StringAlt) {
    Parameter newParam;
    if (thisToken.GetValue() != "") {
        newParam.SetParam(token);
        newParam.SetBool(StringAlt);
        paramList.push_back(newParam);
    }
    else {
    }
    return;
}
Parameter Parser::ParseParameter() {
    Parameter newParam;
    if (fail == false) {
        Token toSend = nextToken;
        if (nextToken.GetType() == STRING) {
            ParseCheck(STRING);
            newParam.SetParam(thisToken.GetValue());
            newParam.SetBool(true);
            return newParam;
        }   
        else if (nextToken.GetType() == ID) {
            ParseCheck(ID);
            newParam.SetParam(thisToken.GetValue());
            newParam.SetBool(false);
            return newParam;
        }
        else if (nextToken.GetType() == LEFT_PAREN) {
            string stringy;
            stringy = ParseExpression();
            stringy = "(" + stringy + ")";
            newParam.SetParam(stringy);
            return newParam;
        }
        else {
            thisToken = nextToken;
            CheckNext(ERROR);
            return newParam;
        }
    }
    else {
        return newParam;
    }
}
void Parser::ParseParameterList() {
    if (fail == false) {
        if (nextToken.GetType() == RIGHT_PAREN) {
            return;
        }
        else {
            Parameter newParam;
            ParseCheck(COMMA);
            newParam = ParseParameter();
            PushOnList(newParam.ToString(), newParam.IsString());
            ParseParameterList();
            return;
        }
    }
    else {
        return;
    }
}
string Parser::ParseExpression() {
    if (fail == false) {
        Expression theExpression;
        Parameter newParam;
        ParseCheck(LEFT_PAREN);
        newParam = ParseParameter();
        theExpression.SetRight(newParam);
        newParam.SetParam(ParseOperator().GetValue());
        theExpression.SetOperator(newParam);
        newParam = ParseParameter();
        theExpression.SetLeft(newParam);
        ParseCheck(RIGHT_PAREN);
        return theExpression.GetExpression();
    }
    else {
        string list;
        return list;
    }
}
Token Parser::ParseOperator() {
    if (fail == false) {
        if (nextToken.GetType() == ADD) {
            ParseCheck(ADD);
        }
        else if (nextToken.GetType() == MULTIPLY) {
            ParseCheck(MULTIPLY);
        }
        else {
            ParseCheck(MULTIPLY);
        }
        return thisToken;
    }
    else {
        return thisToken;
    }
}