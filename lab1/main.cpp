#include <iostream>
#include "Token.h"
#include "Scanner.h"
using namespace std;

int main() {
    Scanner s = Scanner("  ,  ,  ");
    Token t = s.scanToken();
    cout << t.toString() << endl;
}