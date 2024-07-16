#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;


struct Record{
	string name;
	int value;
};

Record symbolTable[50];


int stoi(string from) {
	int to = 0;
	stringstream stream;
	stream << from;
	stream >> to;
	return to;
}

int search (string name) {
	int val = 0;
	cout << "in search" << endl;
	for (int i = 0; i < 50; i++) {
		if (symbolTable[i].name.compare(name) == 0) {
			cout << "name: " << symbolTable[i].name << " value: " << symbolTable[i].value << endl;
			return symbolTable[i].value;
		}
	}
	return 0;
}

int main(void) {
	string line;
	ifstream myFile("./input.txt");
	
	if (myFile.is_open()){
		int i = 0;
		while (getline(myFile, line)) {
			cout << line << '\n';
			size_t position = line.find("=");
			if (position != string::npos) {
				cout << "string size = " << line.size() << " string length = " << line.length() << " = at position " << position << endl;
				symbolTable[i].name = line.substr(0, position - 1);
				cout << "name = " << symbolTable[i].name << endl;
				symbolTable[i].value = stoi(line.substr(position + 1, line.size()));
				cout << "value = " << symbolTable[i].value << endl;
				i++;
			}			
			cout << "ready" << endl;
		}
		myFile.close();
	} else {
		cout <<"unable to open file";
	}
	for (int i = 0; i < 50; i++) {
		cout << symbolTable[i].value << endl;
	}
	int option = 0;
	string options = "Choose option:\n1. Search\n2. Modify\n3. Display all\n4. Exit\n";
	cout << options;
	cin >> option;
	switch (option) {
		case 1:
			{
			
			cout << "Enter name: ";
			string sName;
			cin >> sName;
			cout << "\nValue: " << search << endl;
			break;
		}
		case 2:
		{
		
			cout << "Enter name: ";
			string sName;
			cin >> sName;
			break;
		}
		default:
			return 1;
	}
	
	
	return 0;
}
