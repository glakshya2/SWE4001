#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main(void) {
    ifstream in("source.txt");
    ofstream out("destination.txt");
    if (in.is_open() && out.is_open()) {
        string line;
        while (getline(in, line)) {
            out << line << "\n";
        }
    }
    in.close();
    out.close();
}