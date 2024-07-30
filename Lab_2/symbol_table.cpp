#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

struct record {
	string name;
	int address;
}; 

record[5] table;


void print_full() {
	for (int i = 0; i < 5; i++) {
		
	}
}
int main(void) {
    ifstream in("input.txt");
    int record_index = 0;
    if (in.is_open()) {
        string line;
        while (getline(in, line)) {
            string word;
            stringstream ss(line);
            int i = 0;
            while (ss >> word) {
            	if (i == 0) {
					table[record_index].name = word;
					table[record_index].address = 1000 + (record_index * 3);       			
					i++;
					record_index++;
				}
				
			}
        }
    }
    
    cout << "ready" << endl;
    
    
}
