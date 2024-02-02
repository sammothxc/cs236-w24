#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cctype>
#include <algorithm>
using namespace std;
int spaces = 0;
int letters = 0;
int digits = 0;
int lines = 0;
int characters = 0;
int main(int argc, char* argv[]) {
    ifstream in;
    in.open(argv[1]);
    stringstream ss;
    ss << in.rdbuf();
    string input = ss.str();
    in.close();
    //cout << "*** Start of Input ***" << endl;
    //cout << input << endl;
    //cout << "*** End of Input ***" << endl;
	for (size_t i = 0; i < input.length(); i++) {
		if (input[i] == '\n') {
			lines++;
			characters++;
			spaces++;
		}
		else if (isspace(input[i])) {
			spaces++;
			characters++;
		}
		else if (isalpha(input[i])) {
			letters++;
			characters++;
		}
		else if (isdigit(input[i])) {
			digits++;
			characters++;
		}
		else {
			characters++;
		}
	}
    cout << "chars: " << characters << endl;
    cout << "lines: " << lines << endl;
    cout << "spaces: " << spaces << endl;
    cout << "digits: " << digits << endl;
    cout << "letters: " << letters << endl;
}