#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include "Scanner.h"
#include "Parser.h"
#include "DatalogProgram.h"
#include "Token.h"
#include "Predicate.h"
#include "Rule.h"
#include "Parameter.h"

using namespace std;

int main() {
    string input = "Schemes:\n  snap(S,N,A,P)\n  HasSameAddress(X,Y)\n\nFacts:\n  snap('12345','C. Brown','12 Apple','555-1234').\n  snap('33333','Snoopy','12 Apple','555-1234').\n\nRules:\n  HasSameAddress(X,Y) :- snap(A,X,B,C),snap(D,Y,B,E).\n\nQueries:\n  HasSameAddress('Snoopy',Who)?";

    try {
        Scanner scanner(input);
        Parser parser(scanner.getTokens());
        DatalogProgram program = parser.parse();
        cout << program.toString() << endl;
    } catch (const exception& e) {
        cout << "Failure!" << endl;
        cout << "  (" << e.what() << ")" << endl;
    }

    return 0;
}
