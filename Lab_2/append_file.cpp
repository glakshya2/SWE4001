#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
using namespace std;

const string currentDateTime() {
    time_t now = time(0);
    struct tm tstruct;
    char buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
    return buf;
}

int main(void) {
    ofstream outfile;
    outfile.open("log.txt", ios_base::app);
    outfile << currentDateTime();
    outfile.close();
}