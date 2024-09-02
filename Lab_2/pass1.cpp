#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <iomanip>
#include <vector>

using namespace std;


vector<string> split(string s, string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    string token;
    vector<string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }

    res.push_back (s.substr (pos_start));
    return res;
}


struct Symbol {
    string name;
    int address;
};

int main() {
    ifstream inputFile("input.txt");
    ofstream intermediateFile("intermediate.txt");
    ofstream symtabFile("symtab.txt");

    map<string, Symbol> symtab;
    map<string, int> optab = {
        {"LDA", 3}, {"ADD", 3}, {"SUB", 3}, {"STA", 3}
    };

    int locctr = 0;
    string line, label, opcode, operand;

    getline(inputFile, line);
    label = split(line, "\t")[0];
    opcode = split(line, "\t")[1];
    operand = split(line, "\t")[2];

    if (opcode == "START") {
        locctr = stoi(operand, nullptr, 16);
        intermediateFile << hex << uppercase << setw(4) << setfill('0') << locctr << "\t" << label << "\t" << opcode << "\t" << operand << endl;
        getline(inputFile, line);
    } else {
        locctr = 0;
    }

    int startingAddress = locctr;

    while (true) {
        label = opcode = operand = "";

        label = split(line, "\t")[0];
        opcode = split(line, "\t")[1];
        operand = split(line, "\t")[2];
        if (optab.find(label) != optab.end() || label == "WORD" || label == "RESW" || label == "RESB" || label == "BYTE" || label == "END") {
            opcode = label;
            label = "";
            operand = opcode;
        }

        if (opcode == "END") {
            intermediateFile << "\t" << label << "\t" << opcode << "\t" << operand << endl;
            break;
        }

        if (!label.empty() && label[0] != '.') {
            if (symtab.find(label) != symtab.end()) {
                cerr << "Error: Duplicate symbol " << label << endl;
            } else {
                symtab[label] = {label, locctr};
            }
        }

        intermediateFile << hex << uppercase << setw(4) << setfill('0') << locctr << "\t" << label << "\t" << opcode << "\t" << operand << endl;

        if (optab.find(opcode) != optab.end()) {
            locctr += 3;
        } else if (opcode == "WORD") {
            locctr += 3;
        } else if (opcode == "RESW") {
            locctr += 3 * stoi(operand);
        } else if (opcode == "RESB") {
            locctr += stoi(operand);
        } else if (opcode == "BYTE") {
            if (operand[0] == 'C') {
                locctr += operand.length() - 3;
            } else if (operand[0] == 'X') {
                locctr += (operand.length() - 3) / 2;
            }
        } else {
            cerr << "Error: Invalid opcode " << opcode << endl;
        }

        getline(inputFile, line);
    }

    int programLength = locctr - startingAddress;
    cout << "Program length: " << hex << uppercase << programLength << " bytes" << endl;

    for (const auto& entry : symtab) {
        symtabFile << hex << uppercase << setw(4) << setfill('0') << entry.second.address << "\t" << entry.first << endl;
    }

    inputFile.close();
    intermediateFile.close();
    symtabFile.close();

    return 0;
}
