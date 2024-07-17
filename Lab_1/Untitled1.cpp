#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

struct Record
{
	string name;
	int value;
};

Record symbolTable[50];

// int stoi(string from) {
// 	int to = 0;
// 	stringstream stream;
// 	stream << from;
// 	stream >> to;
// 	return to;
// }

int search(string name)
{
	int val = 0;
	for (int i = 0; i < 50; i++)
	{
		if (symbolTable[i].name.compare(name) == 0)
		{
			return symbolTable[i].value;
		}
	}
	return 0;
}

void modify(string name, int value)
{
	for (int i = 0; i < 50; i++)
	{
		if (symbolTable[i].name.compare(name) == 0)
		{
			symbolTable[i].value = value;
			cout << "Done" << endl;
		}
	}
}
int main(void)
{
	string line;
	ifstream myFile("./input.txt");

	if (myFile.is_open())
	{
		int i = 0;
		while (getline(myFile, line))
		{

			size_t position = line.find("=");
			if (position != string::npos)
			{
				symbolTable[i].name = line.substr(0, position - 1);
				symbolTable[i].value = stoi(line.substr(position + 1, line.size()));
				i++;
			}
			cout << "Ready" << endl;
		}
		myFile.close();
	}
	else
	{
		cout << "unable to open file";
	}
	while (true)
	{
		int option = 0;
		string options = "Choose option:\n1. Search\n2. Modify\n3. Display all\n4. Exit\n";
		cout << options;
		cin >> option;
		switch (option)
		{
		case 1:
		{
			cout << "Enter name: ";
			string name;
			cin >> name;
			cout << "Value: " << search(name) << endl;
			break;
		}
		case 2:
		{
			cout << "Enter name: ";
			string name;
			cin >> name;
			cout << "Enter new value: ";
			int value;
			cin >> value;
			modify(name, value);
			break;
		}
		case 3:
		{
			cout << "Name\tValue" << endl;
			for (int i = 0; i < 50; i++)
			{
				if (symbolTable[i].name.empty()) {
					break;
				}
				cout << symbolTable[i].name << "\t" << symbolTable[i].value << endl;
			}
			break;
		}
		default:
			return 1;
		}
	}

	return 0;
}
