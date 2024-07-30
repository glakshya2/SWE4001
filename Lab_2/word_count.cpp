#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

int main(void) {
	ifstream in("text.txt");
	int count = 0;
	if (in.is_open()) {
		string line;
		while (getline(in, line)) {
			stringstream ss(line);
			string word;
			while (ss >> word) {
				count++;
			}
		}
	}
	cout << count << endl;
	return 0;
}
