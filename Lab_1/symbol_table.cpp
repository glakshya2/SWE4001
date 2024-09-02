#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

struct record
{
    string name;
    int address;
};

record table[5];

void print_full()
{
    cout << "Symbol\tAddress" << endl;
    for (int i = 0; i < 5; i++)
    {
        cout << table[i].name << "\t" << table[i].address << endl;
    }
}

void search_symbol(string symbol)
{
    bool found = false;
    for (int i = 0; i < 5; i++)
    {
        if (table[i].name == symbol)
        {
            cout << "Symbol: " << symbol << ", Address: " << table[i].address << endl;
            found = true;
            break;
        }
    }
    if (!found)
    {
        cout << "Symbol not found" << endl;
    }
}

int main(void)
{
    ifstream in("input.txt");
    int record_index = 0;
    int current_address = 1000;

    if (in.is_open())
    {
        string line;
        while (getline(in, line))
        {
            string word;
            stringstream ss(line);
            int i = 0;
            int multiplier = 0;

            while (ss >> word)
            {
                if (i == 0)
                {
                    table[record_index].name = word;
                    i++;
                }
                else if (i == 1)
                {
                    i++;
                }
                else if (i == 2)
                {
                    multiplier = stoi(word);
                    table[record_index].address = current_address;
                    current_address += multiplier * 3;
                    record_index++;
                }
            }
        }
    }

    cout << "Table ready" << endl;

    int choice;
    while (true)
    {
        cout << "Choose an option:\n1. Print full table\n2. Search for a symbol\n3. Exit\n";
        cin >> choice;

        if (choice == 1)
        {
            print_full();
        }
        else if (choice == 2)
        {
            string symbol;
            cout << "Enter symbol to search: ";
            cin >> symbol;
            search_symbol(symbol);
        }
        else if (choice == 3)
        {
            break;
        }
        else
        {
            cout << "Invalid choice" << endl;
        }
    }

    return 0;
}
