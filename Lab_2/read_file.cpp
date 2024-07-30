#include <iostream>
#include <fstream>
using namespace std;

int main (void) {
	ifstream in;
	in.open("test.txt");
	if (!in) {
		cout << "File not found" << endl;
	} else {
		cout << "File opened" << endl;
	}
	in.close();
}
