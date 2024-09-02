#include <iostream>
#include <fstream>
using namespace std;

int main(void) {
	ifstream in;
	in.open("input.txt");
	if (!in) {
		cout << "File not found" << endl;
	} else {
		char c;
		while (1) {
			in >> c;
			if (in.eof()) {
				break;
			}
			cout << c;
		}
	}
}
