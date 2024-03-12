#include <iostream>
#include <string>
#include <list>
#include <fstream>
#include <vector>
#include "Token.h"
#include "InputStream.h"
#include "Lexer.h"
using namespace std;

char Lexer::advance() {
	return istream->getNext();
}
Token Lexer::getCurrentToken() {
	return *tokenList->at(currentIntToken);
}
Token Lexer::peekNextToken() {
	return *tokenList->at(currentIntToken + 1);
}
void Lexer::startState() {
	state = START;
	currentChar = ' ';
	while (currentChar != EOF) {
		switch (state) {
		case START:
			charSoFar = "";
			currentChar = advance();
			curLineNum = istream->getLineNumber();
			state = parseChar(currentChar);
			break;
		case PUNCUTATION:
			state = START;
			break;
		case COLON:
			state = checkDash();
			break;
		case ID:
			charSoFar += currentChar;
			state = getID();
			break;
		case ERROR:
			break;
		case STRING:
			charSoFar += currentChar;
			state = getString();
			break;
		case END_OF_STRING:
			state = checkEndOfString();
			break;
		case START_OF_COMMENT:
			state = getComment();
			break;
		case SINGLELINE_COMMENT:
			break;
		case MULTILINE_COMMENT:
			state = getMultiComment();
			break;
		case VERTICAL_BAR:
			state = getVerticalBar();
			break;
		default:
			createAndAddToken("UNDEFINED", charSoFar, curLineNum);
			state = START;
			break;
		}
	}
	createAndAddToken("EOF", "");
}
State Lexer::getVerticalBar() {
	if (currentChar == '#') {
		charSoFar += currentChar;
		createAndAddToken("COMMENT", charSoFar, curLineNum);
		return START;
	}
	else {
		charSoFar += currentChar;
		if (currentChar == '|') {
			currentChar = advance();
			return VERTICAL_BAR;
		}
		else {
			return MULTILINE_COMMENT;
		}
	}
}
State Lexer::getMultiComment() {
	while (istream->peek() != EOF && istream->peek() != '|') {
		charSoFar += advance();
	}
	if (istream->peek() == '|') {
		currentChar = advance();
		charSoFar += currentChar;
		currentChar = advance();
		return VERTICAL_BAR;
	}
	return UNDEFINED;
}
State Lexer::getComment() {
	charSoFar += currentChar;
	if (istream->peek() == '|') {
		currentChar = advance();
		charSoFar += currentChar;
		return MULTILINE_COMMENT;
	}
	else {
		while (istream->peek() != EOF && istream->peek() != '\n') {
			charSoFar += advance();
		}
		if (istream->peek() == '\n') {
			return START;
		}
		if (istream->peek() == EOF) {
			return START;
		}
		return UNDEFINED;
	}
}
State Lexer::checkEndOfString() {
	if (currentChar == '\'' && istream->peek() != '\'') {
		createAndAddToken("STRING", charSoFar, curLineNum);
	}
	else {
		currentChar = advance(); // what the hell i don't even know anymore
		return STRING;
	}
	return START;
}
State Lexer::getString() {
	while (istream->peek() != EOF && istream->peek() != '\'') {
		charSoFar += advance();
	}
	if (istream->peek() == '\'') {
		charSoFar += advance();
		return END_OF_STRING;
	}
	else {
		return UNDEFINED;
	}
}
State Lexer::getID() {
	while (istream->peek() != '\n' && istream->peek() != EOF && !isspace(istream->peek())) {
		if (isalnum((unsigned) istream->peek()) || istream->peek() == '_') {
			currentChar = advance();
			charSoFar += currentChar;
		}
		else {
			break;
		}
	}
	if (!isSpecialID()) {
		createAndAddToken("ID", charSoFar);
	}
	return START;
}
bool Lexer::isSpecialID() {
	if (charSoFar == "Facts") {
		createAndAddToken("FACTS", charSoFar);
	}
	else if (charSoFar == "Queries") {
		createAndAddToken("QUERIES", charSoFar);
	}
	else if (charSoFar == "Schemes") {
		createAndAddToken("SCHEMES", charSoFar);
	}
	else if (charSoFar == "Rules") {
		createAndAddToken("RULES", charSoFar);
	}
	else {
		return false;
	}
	return true;
}
State Lexer::checkDash() {
	if (istream->peek() == '-') {
		createAndAddToken("COLON_DASH", charSoFar + advance());
	}
	else {
		createAndAddToken("COLON", charSoFar);
	}
	return START;
}
State Lexer::parseChar(char currentChar) {
	if (isPunct(currentChar)) {
		return PUNCUTATION;
	}
	else if (isGrammarPunct(currentChar)) {
		return PUNCUTATION;
	}
	else if (isWhitespace(currentChar)) {
		return START;
	}
	else if (isColon(currentChar)) {
		return COLON;
	}
	else if (isalpha((unsigned) currentChar)) {
		return ID;
	}
	else if (currentChar == '\'') {
		return STRING;
	}
	else if (currentChar == '#') {
		return START_OF_COMMENT;
	}
	else {
		charSoFar += currentChar;
		return UNDEFINED;
	}
}
Lexer::Lexer(string filename) {
	currentIntToken = 0;
	istream = new InputStream(filename);
	tokenList = new vector<Token*>;
	startState();
	exitAndReturn();
}
void Lexer::exitAndReturn() {
}
bool Lexer::isColon(char currentChar) {
	if (currentChar == ':') {
		charSoFar += currentChar;
		return true;
	}
	return false;
}
string Lexer::parenType(char currentChar) {
	string type;
	if (currentChar == '(') {
		type = "LEFT_PAREN";
	}
	if (currentChar == ')') {
		type = "RIGHT_PAREN";
	}
	else if (currentChar == '*') {
		type = "MULTIPLY";
	}
	else if (currentChar == '+') {
		type = "ADD";
	}
	return type;
}
bool Lexer::isGrammarPunct(char currentChar) {
	string type;
	if (currentChar == '.') {
		type = "PERIOD";
	}
	else if (currentChar == ',') {
		type = "COMMA";
	}
	else if (currentChar == '?') {
		type = "Q_MARK";
	}
	string output = "";
	output += currentChar;
	if (type != "") {
		createAndAddToken(type, output);
	}
	return type != "";
}
bool Lexer::isPunct(char currentChar) {
	string type;
	if (currentChar == '(' || currentChar == ')' || currentChar == '*' || currentChar == '+') {
		type = parenType(currentChar);
	}
	string output = "";
	output += currentChar;
	if (type != "") {
		createAndAddToken(type, output);
	}
	return type != "";
}
bool Lexer::isWhitespace(char currentChar) {
	if (currentChar == '\n' || currentChar == ' ' || currentChar =='\r' || currentChar == '\t' || isspace(currentChar)) {
		return true;
	}
	return false;
}
void Lexer::createAndAddToken(string type, string value, int lineNum) {
	value = "\"" + value + "\"";
	if (lineNum == 0) {
		lineNum = istream->getLineNumber();
	}
	Token* addToken = new Token(type, value, lineNum);
	tokenList->push_back(addToken);
}