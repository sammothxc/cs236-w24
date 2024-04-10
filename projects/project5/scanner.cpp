// ADDED IN PROJECT 1
#include "scanner.h"
#include "token.h"
#include <fstream>
#include <cctype>
#include <cstdio> 
#include <sstream> 

Scanner::Scanner(string nameOfFile) {
    fileName = nameOfFile;
    currentChar = '=';
    nextChar = '=';
    currentLine = 1;
    currentstate = init;
    file.open(fileName);
    facts = "==";
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
        if (!(IsWhiteSpace())) {
            if (newToken.GetType() == COMMENT) {
            }
            else {
                tokenList.push(newToken);
            }
        }
        else if (currentChar == EOF) {
            tokenList.push(newToken);
        }
    }
    return;
}
Token Scanner::StateMachine() {
    Token newToken;
    currentstate = init;
    while (currentstate != sFinal) {
        switch(currentstate) {
            case (init):
                if(currentChar == EOF){
                    currentstate = sEOF;
                }
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

            case (sChar):
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
            case (sSpecial):
                facts = WordExtractor();
                if (currentChar == 'F') {
                    if (facts == "Facts") {
                        newToken.SetValues(facts, currentLine);
                        return newToken;
                    }
                    else {
                        currentstate = sID;
                    }
                }
                else if (currentChar == 'S') {
                    if (facts == "Schemes") {
                        newToken.SetValues(facts, currentLine);
                        return newToken;
                    }
                    else {
                        currentstate = sID;
                    }
                }
                else if (currentChar == 'Q') {
                    if (facts == "Queries") {
                        newToken.SetValues(facts, currentLine);
                        return newToken;
                    }
                    else {
                        currentstate = sID;
                    }
                }
                else if (currentChar == 'R') {
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
            case (sComment):
                nextChar = file.get();
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
                        tokenList.push(newToken);
                        newToken.SetValues("EOF", currentLine + otherlines);
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
            case (sString):
                currentChar = file.get();
                if (currentChar == '\'') {
                    string mstr = "\'";
                    int otherlines = 0;
                    while (currentChar != EOF) {
                        if (currentChar == '\r' || currentChar == '\n') {
                            otherlines++;
                        }
                        nextChar = file.peek();
                        if (currentChar == '\'' && nextChar == '\'') {
                            currentChar = file.get();
                            nextChar = file.peek();
                            mstr = mstr + currentChar + "\'";
                        }
                        else if (currentChar == '\'' && nextChar != '\''){
                            mstr = mstr + "\'";
                            newToken.SetComplexValues("STRING", mstr, currentLine);
                            currentLine = currentLine + otherlines;
                            return newToken;
                            break;
                        }
                        else if (nextChar == EOF) {
                            newToken.SetComplexValues("UNDEFINED", mstr, currentLine);
                            tokenList.push(newToken);
                            newToken.SetValues("EOF", currentLine + otherlines);
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
                        if (currentChar == '\'' && nextChar == '\'') {
                            currentChar = file.get();
                            nextChar = file.peek();
                            mstr = mstr + currentChar + "\'";
                        }
                        else if (currentChar == '\'' && nextChar != '\''){
                            mstr = mstr + "\'";
                            newToken.SetComplexValues("STRING", mstr, currentLine);
                            currentLine = currentLine + otherlines;
                            return newToken;
                            break;
                        }
                        else if (nextChar == EOF) {
                            mstr += currentChar;
                            newToken.SetComplexValues("UNDEFINED", mstr, currentLine);
                            tokenList.push(newToken);
                            newToken.SetValues("EOF", currentLine + otherlines);
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
            case (sID):
                newToken.SetComplexValues("ID", facts, currentLine);
                return newToken;
                break;
            case (sEOF):
                newToken.SetValues("EOF", currentLine);
                return newToken;
                break;
            case (sWhiteSpace):
                currentstate = sFinal;
                break;
            case (sFinal):
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