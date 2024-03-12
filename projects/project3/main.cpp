#include <algorithm>
#include "Lexer.h"
#include "DatalogProgram.h"
#include "Table.h"
using namespace std;

int main(int argc, char *argv[]) {
	Lexer* parse = new Lexer(argv[1]);
	DatalogProgram* program;
	try {
		program = new DatalogProgram(*parse);
		Database db(program);
		db.executeQueries(program);
		delete program;
	}
	catch (const std::runtime_error&){
	}
	delete parse;
	return 0;
}