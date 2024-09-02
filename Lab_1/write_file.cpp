#include <iostream>
#include <fstream>
using namespace std;

int main(void) {
    ofstream outputFile("output.txt");
    outputFile << "Hello World";
    outputFile.close();
}