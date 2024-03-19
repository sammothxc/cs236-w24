#include "Scanner.h"
#include "Token.h"
#include "DatalogProgram.h"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

int main(int argc, char *argv[]) {
  Scanner scanner;
  vector<Token> scannedTokens;

  if (argv[1]){
    scannedTokens = scanner.scan(argv[1]);

    try {
      DatalogProgram dP = DatalogProgram(scannedTokens);
      cout<<"Success!"<<endl;
      cout<<dP.toString()<<endl;
    } catch( Token token ){
      cout<<"Failure!"<<endl;
      cout<<"  "<<token.output()<<endl;
    }
  } else {
    cout<<"You need to enter a file name after ./[compiled_code]"<<endl;
  }
  /************* TESTS *************/

  /*else {

    cout<<"Test 1 (Success!) Output:"<<endl;
    try {
      datalogProgram.parse(scanner.scan("pass1.txt"));
      cout<<"Success!"<<endl<<endl;
    } catch( Token token ){
      cout<<"Failure!"<<endl;
      cout<<"  "<<token.output()<<endl<<endl;
    }

    DatalogProgram datalogProgram2;
    Scanner scanner2;
    scannedTokens = scanner2.scan("pass2.txt");
    cout<<"Test 2 (Success!) Output:"<<endl;
    try {
      datalogProgram2.parse(scannedTokens);
      cout<<"Success!"<<endl<<endl;
    } catch( Token token ){
      cout<<"Failure!"<<endl;
      cout<<"  "<<token.output()<<endl<<endl;
    }

    DatalogProgram datalogProgram3;
    Scanner scanner3;
    scannedTokens = scanner3.scan("pass3.txt");
    cout<<"Test 3 (Success!) Output:"<<endl;
    try {
      datalogProgram3.parse(scannedTokens);
      cout<<"Success!"<<endl<<endl;
    } catch( Token token ){
      cout<<"Failure!"<<endl;
      cout<<"  "<<token.output()<<endl<<endl;
    }

    DatalogProgram datalogProgram4;
    Scanner scanner4;
    scannedTokens = scanner4.scan("pass4.txt");
    cout<<"Test 4 (Success!) Output:"<<endl;
    try {
      datalogProgram4.parse(scannedTokens);
      cout<<"Success!"<<endl<<endl;
    } catch( Token token ){
      cout<<"Failure!"<<endl;
      cout<<"  "<<token.output()<<endl<<endl;
    }

    DatalogProgram datalogProgram5;
    Scanner scanner5;
    scannedTokens = scanner5.scan("fail1.txt");
    cout<<"Test 5 (Failure!) Output:"<<endl;
    try {
      datalogProgram5.parse(scannedTokens);
      cout<<"Success!"<<endl;
    } catch( Token token ){
      cout<<"Failure!"<<endl;
      cout<<"  "<<token.output()<<endl<<endl;
    }

    DatalogProgram datalogProgram6;
    Scanner scanner6;
    scannedTokens = scanner6.scan("fail2.txt");
    cout<<"Test 6 (Failure!) Output:"<<endl;
    try {
      datalogProgram6.parse(scannedTokens);
      cout<<"Success!"<<endl;
    } catch( Token token ){
      cout<<"Failure!"<<endl;
      cout<<"  "<<token.output()<<endl<<endl;
    }

    DatalogProgram datalogProgram7;
    Scanner scanner7;
    scannedTokens = scanner7.scan("fail3.txt");
    cout<<"Test 7 (Failure!) Output:"<<endl;
    try {
      datalogProgram7.parse(scannedTokens);
      cout<<"Success!"<<endl;
    } catch( Token token ){
      cout<<"Failure!"<<endl;
      cout<<"  "<<token.output()<<endl<<endl;
    }

    DatalogProgram datalogProgram8;
    Scanner scanner8;
    scannedTokens = scanner8.scan("fail4.txt");
    cout<<"Test 8 (Failure!) Output:"<<endl;
    try {
      datalogProgram8.parse(scannedTokens);
      cout<<"Success!"<<endl;
    } catch( Token token ){
      cout<<"Failure!"<<endl;
      cout<<"  "<<token.output()<<endl<<endl;
    }

    DatalogProgram datalogProgram9;
    Scanner scanner9;
    scannedTokens = scanner9.scan("fail5.txt");
    cout<<"Test 9 (Failure!) Output:"<<endl;
    try {
      datalogProgram9.parse(scannedTokens);
      cout<<"Success!"<<endl;
    } catch( Token token ){
      cout<<"Failure!"<<endl;
      cout<<"  "<<token.output()<<endl<<endl;
    }

    DatalogProgram datalogProgram10;
    Scanner scanner10;
    scannedTokens = scanner10.scan("fail6.txt");
    cout<<"Test 10 (Failure!) Output:"<<endl;
    try {
      datalogProgram10.parse(scannedTokens);
      cout<<"Success!"<<endl;
    } catch( Token token ){
      cout<<"Failure!"<<endl;
      cout<<"  "<<token.output()<<endl<<endl;
    }
  }*/
}
