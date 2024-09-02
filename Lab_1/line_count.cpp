#include <iostream>
#include <fstream>
using namespace std;

int main (void) {
    ifstream in("data.txt");
    int count = 0;
    if (in.is_open()) {
        string line;
        while (getline(in, line)) {
             count++;
        }
    }
    cout << count;
}