#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <set>
#include <cctype>
#include <algorithm>
#include <cstdio>
#include <queue>
#include "DatalogProgram.h"
#include "Expression.h"
#include "Parameter.h"
#include "Parser.h"
#include "Predicate.h"
#include "Rules.h"
#include "Scanner.h"
#include "Token.h"

using namespace std;

vector<Rule> Datalog::GetRules() {
    return Rules;
}
vector<Predicate> Datalog::GetQueries() {
    return Queries;
}
vector<Predicate> Datalog::GetFacts() {
    return Facts;
}
vector<Predicate> Datalog::GetSchemes() {
    return Schemes;
}
void Datalog::SetDomain(set<string> Set) {
    Domain = Set;
}
void Datalog::SetFacts(vector<Predicate> Fact) {
    for (unsigned int i = 0; i < Fact.size(); i++) {
        Facts.push_back(Fact.at(i));
    }
}
void Datalog::SetQueries(vector<Predicate> Query) {
    for (unsigned int i = 0; i < Query.size(); i++) {
        Queries.push_back(Query.at(i));
    }
}
void Datalog::SetSchemes(vector<Predicate> Scheme) {
    for (unsigned int i = 0; i < Scheme.size(); i++) {
        Schemes.push_back(Scheme.at(i));
    }
}
void Datalog::SetRules(vector<Rule> Rule) {
    for (unsigned int i = 0; i < Rule.size(); i++) {
        Rules.push_back(Rule.at(i));
    }
}
string Datalog::ToString() {
    stringstream ss;
    ss << "Schemes(" << Schemes.size() << "):\n";
    for (unsigned int i = 0; i< Schemes.size();i++) {
        ss<< "  " << Schemes.at(i).ToString() << "\n";
    }
    ss << "Facts(" << Facts.size() << "):\n";
    for (unsigned int i = 0; i< Facts.size();i++) {
        ss<< "  " << Facts.at(i).ToString() << ".\n";
    }
    ss << "Rules(" << Rules.size() << "):\n";
    for (unsigned int i = 0; i< Rules.size();i++) {
        ss<< "  " << Rules.at(i).ToString() << "\n";
    }
    ss << "Queries(" << Queries.size() << "):\n";
    for (unsigned int i = 0; i< Queries.size();i++) {
        ss<< "  " << Queries.at(i).ToString() << "?\n";
    }
    ss << "Domain(" << Domain.size() << "):\n";
    set<string>::iterator it;
    for (it = Domain.begin(); it != Domain.end(); it++) {
        ss << "  " << *it << "\n";
    }

    string thing = ss.str();
    return thing;
}

void Expression::SetRight(Parameter right) {
    rightParam = right;
}
void Expression:: SetOperator(Parameter oper) {
    Operator = oper;
}
void Expression::SetLeft(Parameter left) {
    leftParam = left;
}
string Expression::GetExpression(){
    string Express;
    Express = rightParam.ToString() + Operator.ToString() + leftParam.ToString();
    //cout << Express;
    return Express;
}


string Expression::ToString() {
    cout << "Expression to String" << endl;
    string stringy = "(" + rightParam.ToString() + Operator.ToString() + leftParam.ToString() + ")";
    return stringy;
}

void Parameter::SetParam(string Toke){
    theParameter = Toke;
}

string Parameter::ToString(){
    return theParameter;
}

Parser::Parser() {}

Parser::~Parser(){}

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
    ParseCheck(SCHEMES); //Schemes
    ParseCheck(COLON);
    Schemes.push_back(ParseScheme());
    Schemes = ParseSchemeList(Schemes);
    if (fail == false) {
        Everything.SetSchemes(Schemes);
        //cout << "Success! Schemes" << endl;/////////////////////////////////////
    }
    else {
        return Everything;
    }

    if(fail == false) {
        ParseCheck(FACTS);//Facts
        ParseCheck(COLON);
        vector<Predicate> parsedFacts; 
        parsedFacts = ParseFactList(parsedFacts);
        if (fail == false) {
            Everything.SetFacts(parsedFacts);
            Everything.SetDomain(Domain);
            //cout << "Success! Facts" << endl;///////////////////////////////////////
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
        ParseCheck(RULES);//Rules
        ParseCheck(COLON);
        Rules = ParseRuleList(Rules);
        if (fail == false) {
            Everything.SetRules(Rules);
            //cout << "Success! Rules" << endl;///////////////////////////////////////
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
            //cout << "success! Queries" << endl;///////////////////////////////////////
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
            cout << "Success!" << endl;
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

vector<Predicate> Parser::ParseQueryList(vector<Predicate> Queries) {//lambda // NEEEEEEEDS WORK????????
    if (fail == false) {
        if (nextToken.GetType() != ID) {
            return Queries;
        }
        /*I feel like there is a chance that something wrong is going on
        like if the id but not the right thing*/
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

vector<Rule> Parser::ParseRuleList(vector<Rule> RuleList) {// check for lambda
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

vector<Predicate> Parser::ParseFactList(vector<Predicate> Facts) { // needs to check for lambda
    //cout << "FactList" << endl;/////////////////////////////////////////////////////////
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
        PushOnList(thisToken.GetValue());
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

void Parser::ParseStringList(){ //check for lambda
    //cout << "string list" << endl;//////////////////////////////////////////////////////////
    if (fail == false) {
        if (nextToken.GetType() == COMMA) {
            ParseCheck(COMMA);
            ParseCheck(STRING);
            PushOnList(thisToken.GetValue());
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
        ParseCheck(ID); ////////////////////////////////////////////////////////////////////
        Schemes.SetName(thisToken.GetValue());
        ParseCheck(LEFT_PAREN);
        ParseCheck(ID);
        PushOnList(thisToken.GetValue());
        ParseIDList();
        ParseCheck(RIGHT_PAREN);
        for (unsigned int i = 0; i < paramList.size(); i++) {
            Schemes.PushPredicate(paramList.at(i));
        }
        //cout << Schemes.ToString() << endl;;
        return Schemes;
    }
    else {
        return Schemes;
    }
}

vector<Predicate> Parser::ParseSchemeList(vector<Predicate> Schemes) { //check for lambda??
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

void Parser::ParseIDList() {//check for lambda???
    //cout << "Id list" << endl;//////////////////////////////////////////
    if (fail == false) {
        if (nextToken.GetType() == COMMA) {
            ParseCheck(COMMA);
            ParseCheck(ID);
            PushOnList(thisToken.GetValue());
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
    if (fail == false) { // syntax may need to be changed to fail != true
        if (nextToken.GetType() != EOFa) {
            thisToken = TokenQueue.front();
            TokenQueue.pop();
            nextToken = TokenQueue.front();
            //cout << thisToken.toString() << endl;////////////////////////////////////////////////////////////////////////////
            if (thisToken.GetType() == type) {
                return true; 
            }
            else {
                PrintFail(thisToken.GetLineNumber());
                return false;
            }
        }
        else {//////////////////////////////////////////////////////////////////////////
            thisToken = TokenQueue.front();
            //cout << "we are here" << endl;/////////////////////////////////////////////
            //TokenQueue.pop();///////////////////////////////////////////////////////////
            return true;////////////////////////////////////////////////////////////////
        }
    }
    else {
        if (nextToken.GetType() != EOFa) {
            //cout << "we are here?" << endl;
            thisToken = TokenQueue.front();
            //cout << thisToken.toString() << endl;
            TokenQueue.pop();
            nextToken = TokenQueue.front();
            //cout << "we are here?" << endl;
        }
        else {
            thisToken = TokenQueue.front();
            //cout << "we are here" << endl;
        }
        return false;
    }
}

void Parser::PrintFail(int i) {
    cout << "Failure!" << "\n  (" << thisToken.TypeString() << ",\"" <<thisToken.GetValue() <<"\"," << i << ")" << endl;
    fail = true;
    return;
}

//Predicate Stuff
Predicate Parser::ParseHeadPredicate() {
    //cout << "Head predicate" << endl;////////////////////////////////////////////
    Predicate headPredicate;
    if (fail == false) {
        paramList.clear();
        ParseCheck(ID);
        headPredicate.SetName(thisToken.GetValue());
        ParseCheck(LEFT_PAREN);
        ParseCheck(ID);
        PushOnList(thisToken.GetValue());
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

vector<Predicate> Parser::ParsePredicateList(vector<Predicate> list) { //lamda
    //cout << "PredicateList" << endl;///////////////////////////////////////////////
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
    //cout << "Predicate" << endl;/////////////////////////////////////////////////////
    Predicate Predicates;
    Parameter newParam;
    if (fail == false) {
        paramList.clear();
        ParseCheck(ID);
        Predicates.SetName(thisToken.GetValue());
        //PushOnList(thisToken.GetValue());
        ParseCheck(LEFT_PAREN);
        newParam = ParseParameter();
        PushOnList(newParam.ToString());
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

void Parser::PushOnList(string token) {
    Parameter newParam;
    if (thisToken.GetValue() != "") {
        newParam.SetParam(token);
        paramList.push_back(newParam);
    }
    else {
    }
    return;
}

Parameter Parser::ParseParameter() {
    //cout << "Parameter" << endl;//////////////////////////////////////////////////////
    Parameter newParam;
    if (fail == false) {
        Token toSend = nextToken;
        if (nextToken.GetType() == STRING) {
            //cout << "string";
            ParseCheck(STRING);
            newParam.SetParam(thisToken.GetValue());
            return newParam;
        }   
        else if (nextToken.GetType() == ID) {
            //cout << "ID";
            ParseCheck(ID);
            newParam.SetParam(thisToken.GetValue());
            return newParam;
        }
        else if (nextToken.GetType() == LEFT_PAREN) {
            string stringy;
            stringy = ParseExpression();
            stringy = "(" + stringy + ")";
            newParam.SetParam(stringy);
            //CheckNext(RIGHT_PAREN);////////////////////
            //cout << newParam.ToString();
            return newParam;
        }
        //check --------------------------------------------------------------------------------------------
        else {
            thisToken = nextToken;
            CheckNext(ERROR);/////////////////////////////////////////////////////////////////////////////////////////
            return newParam;
        }
    }
    else {
        return newParam;
    }
}

void Parser::ParseParameterList() { // lambda
    //cout << "Parameter List" << endl;/////////////////////////////////////////////////////////
    if (fail == false) {
        if (nextToken.GetType() == RIGHT_PAREN) {
            return;
        }
        else {
            Parameter newParam;
            ParseCheck(COMMA);
            newParam = ParseParameter();
            PushOnList(newParam.ToString());
            ParseParameterList();
            return;
        }
    }
    else {
        return;
    }
}

string Parser::ParseExpression() {
    //cout << "Expression" << endl;
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
        //ParseParameter();
        ParseCheck(RIGHT_PAREN);

        return theExpression.GetExpression();
    }
    else {
        string list;
        return list;
    }
}

Token Parser::ParseOperator() {
    //cout << "Operator" << endl;/////////////////////////////////////////////////////////////////////
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

void Predicate::SetName(string theName) {
    Name = theName;
}

void Predicate::PushPredicate(Parameter theParameter) {
    parameterList.push_back(theParameter);
    return;
}
string Predicate::ToString() {
    string Pstring = Name + "(";
    for (unsigned int i = 0; i < parameterList.size(); i++) {
        
        if (i ==0 ){
            Pstring = Pstring + parameterList.at(i).ToString();
        }
        else {
            Pstring = Pstring + "," + parameterList.at(i).ToString();
        }
    }
    Pstring = Pstring + ")";
    return Pstring;
}

vector<Parameter> Predicate::ReturnVector() {
    return parameterList;
}

void Rule::SetHeadPredicate(Predicate head) {
    Head = head;
}

void Rule::SetRule(vector<Predicate> Toke) {
    for (unsigned int i = 0; i < Toke.size(); i++) {
        RuleList.push_back(Toke.at(i));
    }
}

string Rule::ToString() {
    string theRule;
    theRule = Head.ToString() + " :- ";
    for (unsigned int i = 0; i < RuleList.size(); i++) {
        if ( i == 0) {
            theRule = theRule + RuleList.at(i).ToString();
        }
        else {
            theRule = theRule + "," + RuleList.at(i).ToString();
        }

    } 
    theRule += ".";
    return theRule;
}

Scanner::Scanner(string nameOfFile) {
    fileName = nameOfFile;
    currentChar = '=';
    nextChar = '=';
    currentLine = 1;
    currentstate = init;
    file.open(fileName);
    facts = "==";
    //initial = 1;
}

Scanner::~Scanner() {
    file.close();
}

bool Scanner::IsWhiteSpace() {
    if (currentChar == ' ' || currentChar == '\t' || currentChar == '\n' || currentChar == '\r') {
        return true;
    }
    else {
        return false;
    }
}


string Scanner::WordExtractor() {
    string newWord;
    char wordChar;
    newWord += currentChar;  
    wordChar = file.get();
    while(isalpha(wordChar)||isdigit(wordChar)) {
        newWord = newWord + wordChar;
        wordChar = file.get();  
    }
    if (!(isalpha(wordChar)) && !isdigit(wordChar)) {
        nextCharVal = true;
        nextChar = wordChar;
    }            
    return newWord;
}

int Scanner::LineGenerator() {
    currentLine++;
    return currentLine;
}

void Scanner::tokenize() {
    while (file.good()) {
        Token newToken;
        if (nextCharVal == true) {
            currentChar = nextChar;
            nextCharVal = false;
        }
        else {
            currentChar = file.get();
        }
        newToken = StateMachine();
        if /*(*/(!(IsWhiteSpace()) /*&& initial == 0)*/) { // change to newline and stuff that doesn't make a token
            if (newToken.GetType() == COMMENT) {
                //cout << "Comment Ignored" << endl;
            }
            else {
                tokenList.push(newToken);
            }
        }
        else if (currentChar == EOF) {
            tokenList.push(newToken);
        }
        //initial = 0;
    }
    return;
}

Token Scanner::StateMachine() {
    Token newToken;
    currentstate = init;
    while (currentstate != sFinal) {
        switch(currentstate) {
            case (init): // ----------------------------------------------- init CASE ---------------------------------------------------
                if(currentChar == EOF){
                    currentstate = sEOF;
                }
                /*
                else if (initial == 1) {
                    currentstate = sFinal;
                }
                */
                else if (currentChar == ','|| currentChar == '.'|| currentChar == '?'||
                    currentChar == '('|| currentChar == ')'|| currentChar == '*'||
                    currentChar == '+'|| currentChar == ':') {
                    currentstate = sChar; 
                }
                else if (IsWhiteSpace()) {
                    if (currentChar == '\n'|| currentChar == '\r') {
                        currentLine++;
                        currentstate = sWhiteSpace;
                    }
                    else if (currentChar == '\t') {
                        currentstate = sWhiteSpace;
                    }
                    currentstate = sWhiteSpace;
                } 
                else if (currentChar == '#') {
                    currentstate = sComment;
                }
                else if (currentChar == '\'') {
                    currentstate = sString;
                }
                else if (isalpha(currentChar)) {
                    currentstate = sSpecial;
                }
                else {
                    string mstr;
                    mstr += currentChar;
                    newToken.SetComplexValues("UNDEFINED", mstr, currentLine);
                    return newToken;
                }
                break;

            case (sChar): //----------------------------------------------- sChar CASE -----------------------------------------------------
            // ---------------------------------------- Identifies ":" ":-" "," "." "(" ")" "*" "+" ----------------------------------------
                if (currentChar == ':') {
                    nextChar = file.get();
                    if (nextChar == '-') {
                        newToken.SetValues(":-", currentLine);
                        return newToken;
                    }
                    else {
                        newToken.SetValues(":", currentLine);
                        nextCharVal = true;
                        return newToken;
                    }
                }
                else {
                    string mystring;
                    mystring += currentChar;
                    newToken.SetValues(mystring, currentLine);
                    return newToken;
                }
                break;

            case (sSpecial): // ----------------------------------------- sSpecial CASE ----------------------------------------------------
            // -----------------------------------------------I dentifies the "Facts", "Queries", "Rules", "Schemes" -----------------------
                facts = WordExtractor();
                if (currentChar == 'F') { // THIS IS TO IDENTIFY THE FACTS KEYWORD
                    if (facts == "Facts") {
                        newToken.SetValues(facts, currentLine);
                        return newToken;
                    }
                    else {
                        currentstate = sID;
                    }
                }
                else if (currentChar == 'S') { // THIS IS TO IDENTIFY THE SCHEMES KEYWORD
                    if (facts == "Schemes") {
                        newToken.SetValues(facts, currentLine);
                        return newToken;
                    }
                    else {
                        currentstate = sID;
                    }
                }
                else if (currentChar == 'Q') { // THIS IS TO IDENTIFY THE QUERIES KEYWORD
                    if (facts == "Queries") {
                        newToken.SetValues(facts, currentLine);
                        return newToken;
                    }
                    else {
                        currentstate = sID;
                    }
                }
                else if (currentChar == 'R') { // THIS IS TO IDENTIFY THE RULES KEY WORD
                    if (facts == "Rules") {
                        newToken.SetValues(facts, currentLine);
                        return newToken;
                    }
                    else {
                        currentstate = sID;
                    }
                }
                else {
                    currentstate = sID;
                }
                break;

            case (sComment)://---------------------------------------------------sComment-------------------------------------------------
            // not working for the special case that it reaches the end without getting the closing comment identifier
                nextChar = file.get();
                //block comment
                currentChar = nextChar;
                if (currentChar == '|') {
                    int otherlines = 0;
                    bool isEOF = false;
                    string bcom;
                    bcom = "#|";
                    currentChar = file.get();
                    nextChar = file.peek();
                    while (currentChar != EOF || nextChar != EOF) {
                        if (nextChar == '#' && currentChar == '|') {
                            bcom += "|#";
                            file.get();
                            newToken.SetComplexValues("COMMENT", bcom, currentLine);
                            currentLine =  currentLine + otherlines;
                            return newToken;
                        }
                        else if (nextChar == EOF) {
                            isEOF = true;
                        }

                        if (currentChar == '\r' || currentChar == '\n'){
                            otherlines = otherlines + 1;
                            bcom.append("\n");
                        }
                        else {
                            string cool;
                            cool += currentChar;
                            bcom.append(cool);
                        }
                        currentChar = file.get();
                        nextChar = file.peek();
                    }
                    if (isEOF) {
                        newToken.SetComplexValues("UNDEFINED", bcom, currentLine);
                        tokenList.push(newToken);//////////////////////////////////////////////////////////////////////////////////////////////
                        newToken.SetValues("EOF", currentLine + otherlines);/////////////////////////////////////////////////////////////////////////////////
                        currentChar = EOF;
                       return newToken;
                    }
                    else {
                        newToken.SetComplexValues("COMMENT", bcom, currentLine);
                        currentLine = currentLine + otherlines;
                        newToken.toString();
                        return newToken;
                    }
                    currentChar = file.get();
                }
                //line comment
                else {
                    string comment = "#";
                    comment = comment + currentChar;
                    nextChar = file.peek();
                    while (nextChar != '\n') {
                        currentChar = file.get();
                        comment = comment + currentChar;
                        nextChar = file.peek();
                        if (nextChar == '\r' || nextChar =='\n') {
                            break;
                        }
                    }
                    newToken.SetComplexValues("COMMENT", comment, currentLine);
                    return newToken;
                }
                
                
            break;
            case (sString)://-----------------------------------------------------sString CASE -------------------------------------
                currentChar = file.get();
                //general case: goes until reaches another '
                if (currentChar == '\'') {
                    string mstr = "\'";
                    int otherlines = 0;
                    while (currentChar != EOF) {
                        if (currentChar == '\r' || currentChar == '\n') {
                            otherlines++;
                        }
                        nextChar = file.peek();
                        if (currentChar == '\'' && nextChar == '\'') { // special case that back to back '' are found
                            currentChar = file.get();
                            nextChar = file.peek();
                            mstr = mstr + currentChar + "\'";

                        }
                        else if (currentChar == '\'' && nextChar != '\''){  // single ' is found
                            mstr = mstr + "\'";
                            newToken.SetComplexValues("STRING", mstr, currentLine);
                            currentLine = currentLine + otherlines;
                            return newToken;
                            break;
                        }
                        else if (nextChar == EOF) {
                            newToken.SetComplexValues("UNDEFINED", mstr, currentLine);
                            tokenList.push(newToken);/////////////////////////////////////////////////////////////////////
                            newToken.SetValues("EOF", currentLine + otherlines);////////////////////////////////////////////////////////
                            currentChar = EOF;
                            return newToken;
                            break;
                        }
                        else {
                            mstr = mstr + currentChar;
                        }
                        currentChar = file.get();
                    }
                }
                else if (currentChar == '\'') {
                    newToken.SetComplexValues("STRING", "\'\'", currentLine);
                    return newToken;
                }
                else {
                    string mstr = "\'";
                    int otherlines = 0;
                    while (currentChar != EOF) {
                        if (currentChar == '\r' || currentChar == '\n') {
                            otherlines++;
                        }
                        nextChar = file.peek();
                        if (currentChar == '\'' && nextChar == '\'') { // special case that back to back '' are found
                            currentChar = file.get();
                            nextChar = file.peek();
                            mstr = mstr + currentChar + "\'";

                        }
                        else if (currentChar == '\'' && nextChar != '\''){  // single ' is found
                            mstr = mstr + "\'";
                            newToken.SetComplexValues("STRING", mstr, currentLine);
                            currentLine = currentLine + otherlines;
                            return newToken;
                            break;
                        }
                        else if (nextChar == EOF) {
                            mstr += currentChar;
                            newToken.SetComplexValues("UNDEFINED", mstr, currentLine);
                            tokenList.push(newToken);/////////////////////////////////////////////////////////////////////
                            newToken.SetValues("EOF", currentLine + otherlines);////////////////////////////////////////////////////////
                            currentChar = EOF;
                            return newToken;
                            break;
                        }
                        else {
                            mstr = mstr + currentChar;
                        }
                        currentChar = file.get();
                    }
                }
            break;
            case (sID): //---------------------------------------------------------- sID CASE ---------------------------------------
                newToken.SetComplexValues("ID", facts, currentLine);
                return newToken;
                break;
            case (sEOF): // -------------------------------------------------------- EOF CASE ---------------------------------------
                newToken.SetValues("EOF", currentLine);
                return newToken;
                break;
            case (sWhiteSpace): //-------------------------------------------------- WhiteSpace CASE --------------------------------
                currentstate = sFinal;
                break;
            case (sFinal): // ------------------------------------------------------ sFinal CASE ------------------------------------
                return newToken;
                break;
            default:
                newToken.SetComplexValues("UNDEFINED", "UNDEFINED", currentLine);
        }   
    }
    newToken.SetComplexValues("UNDEFINED", "UNDEFINED", currentLine);
    return newToken;
}

queue <Token> Scanner::GetList() {
    return tokenList;
}

Token::Token() {
    tokenValue = "BEN FOGG";
    lineNumber = -1;
    tokenType = ERROR;
}

Token::~Token() {
}

int Token::GetLineNumber(){
    return lineNumber;
}

TokenType Token::GetType() {
    return tokenType;
}

string Token::toString() {
    string theToken;
    string type = "ERROR1";
    switch (tokenType) {
        case COMMA:
            type = "COMMA";
            break;
        case PERIOD:
            type = "PERIOD";
            break;
        case Q_MARK:
            type = "Q_MARK";
            break;
        case LEFT_PAREN:
            type = "LEFT_PAREN";
            break;
        case RIGHT_PAREN:
            type = "RIGHT_PAREN";
            break;
        case COLON:
            type = "COLON";
            break;
        case COLON_DASH:
            type = "COLON_DASH";
            break;
        case MULTIPLY:
            type = "MULTIPLY";
            break;
        case ADD: 
            type = "ADD";
            break;
        case SCHEMES:
            type = "SCHEMES";
            break;
        case FACTS:
            type = "FACTS";
            break;
        case RULES:
            type = "RULES";
            break;
        case QUERIES:
            type = "QUERIES";
            break;
        case ID:
            type = "ID";
            break;
        case STRING:
            type = "STRING";
            break;
        case COMMENT:
            type = "COMMENT";
            break;
        case UNDEFINED:
            type = "UNDEFINED";
            break;
        case EOFa:
            type = "EOF";
            break;
        case ERROR:
            type = "ERROR";
    }
    theToken = "(" + type + ",\"" + tokenValue + "\"," + to_string(lineNumber) + ")";
    return theToken;
}

void Token::SetValues(string myString ,int lineNum) {
        lineNumber = lineNum;
        tokenValue = myString;
    if (myString == ",") {
        tokenType = COMMA;
    }
    else if (myString == ".") {
        tokenType = PERIOD;
    }
    else if (myString == "?") {
        tokenType = Q_MARK;
    }
    else if (myString == "(") {
        tokenType = LEFT_PAREN;
    }
    else if (myString == ")") {
        tokenType = RIGHT_PAREN;
    }
    else if (myString == ":-") {
        tokenType = COLON_DASH;
    }
    else if (myString == ":") {
        tokenType = COLON;
    }
    else if (myString == "*") {
        tokenType = MULTIPLY;
    }
    else if (myString == "+") {
        tokenType = ADD;
    }
    else if (myString == "Schemes") {
        tokenType = SCHEMES;
    }
    else if (myString == "Facts") {
        tokenType = FACTS;
    }
    else if (myString == "Queries") {
        tokenType = QUERIES;
    }
    else if (myString == "Rules") {
        tokenType = RULES;
    }
    else if (myString == "EOF") {
        tokenType = EOFa;
        tokenValue = "";
    }
    //NEED TO ADD STRINGS AND COMMENTS and IDs
    else {
        tokenType = UNDEFINED;
    }

}

void Token::SetComplexValues (string type, string token, int lineNum) {
    if (type == "ID") {
        tokenType = ID;
        tokenValue = token;
        lineNumber = lineNum;
    }
    else if ( type == "COMMENT") {
        tokenType = COMMENT;
        tokenValue = token; // may need to be modified
        lineNumber = lineNum;
    }
    else if (type == "STRING") {
        tokenType = STRING;
        tokenValue = token; // may need ot be modified
        lineNumber = lineNum;

    }
    else if (type == "UNDEFINED") {
        tokenType = UNDEFINED;
        tokenValue = token; // may need to be modified
        lineNumber = lineNum;

    }
}

string Token::TypeString() {
    string type = "ERROR1";
    switch (tokenType) {
        case COMMA:
            type = "COMMA";
            break;
        case PERIOD:
            type = "PERIOD";
            break;
        case Q_MARK:
            type = "Q_MARK";
            break;
        case LEFT_PAREN:
            type = "LEFT_PAREN";
            break;
        case RIGHT_PAREN:
            type = "RIGHT_PAREN";
            break;
        case COLON:
            type = "COLON";
            break;
        case COLON_DASH:
            type = "COLON_DASH";
            break;
        case MULTIPLY:
            type = "MULTIPLY";
            break;
        case ADD: 
            type = "ADD";
            break;
        case SCHEMES:
            type = "SCHEMES";
            break;
        case FACTS:
            type = "FACTS";
            break;
        case RULES:
            type = "RULES";
            break;
        case QUERIES:
            type = "QUERIES";
            break;
        case ID:
            type = "ID";
            break;
        case STRING:
            type = "STRING";
            break;
        case COMMENT:
            type = "COMMENT";
            break;
        case UNDEFINED:
            type = "UNDEFINED";
            break;
        case EOFa:
            type = "EOF";
            break;
        case ERROR:
            type = "ERROR";
        break;
    }
    return type;
}

string Token::GetValue() {
    return tokenValue;
}

int main (int argc, char* argv[]) {
    if (!argv[1]) {
        cout << "must include file name in commandline" << endl;
        cout << "Total Tokens = 0" << endl;
    }
    else {
        string filename = argv[1];
        Scanner myScanner(filename);  
        myScanner.tokenize();
        Parser myParser;
        bool trueorfalse = myParser.DatalogProgram(myScanner.GetList());
        if (trueorfalse == false) {
            myParser.ToString();
        }
    }
    return 0;
}