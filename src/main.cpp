// std
#include <iostream>
#include <string>
#include <fstream>
#include <exception>

// costom
#include "determin_skip_list.h"

using namespace std;
using namespace skip_list;

/**
 * TestSkipList is the kernal test fuction of Insert,Search and Remove of a
 * determinstic skip list.
 * @param skip_list is the user created empty skip list for test
 * @param in is the ifstream relating to the opened file
 * @param out is the ofstream relating to the file to write result
 */
template <typename T>
void TestSkipList(DeterminSkipList<T>& skip_list, ifstream& in, ofstream& out)
{
    string op;
    string value;
    ///loop to get input and to corresponding operation on skip_list
    ///and write the result to the out put file
    while (in >> op >> value) {
        if (op == "INS") {
            if (skip_list.Insert(value))
                out << op << " " << value << endl;
            else
                out << "DUP" << " " << value << endl;
        }
        else if (op == "FIND") {
            if (skip_list.Search(value))
                out << op << " " << value << endl;
            else
                out << "NONE" << " " << value << endl;
        }
        else if (op == "REM") {
            if (skip_list.Remove(value))
                out << op << " " << value << endl;
            else
                out << "NONE" << " " << value << endl;
        }
        if (!skip_list.IsValid())
            cout << "not valid for " << op << " " << value << endl;
    }
}

int main(int argc, char* argv[])
{
    if (argc != 3) {
        cout << "Usage is : "
            << "test_determin_skip_list <input_file> <output_file>"
            << endl;
        return -1;
    }
    ///argv[1] the name of the file to open
    ///argv[2] the name of the file to wirte
    ifstream in(argv[1]);
    if (!in) {
        cout << "Could not find the file named "
            << argv[1] << " to open!" << endl;
        return -1;
    }
    ofstream out(argv[2]);
    ///create a empty skip list key type is string,
    ///assumming for any input key value
    ///key value < Max < Max_1
    char a[2];
    a[0] = char(254); a[1] = '\0';
    const string Max(a);
    a[0] = char(255);
    const string Max_1(a);
    DeterminSkipList<string> skip_list(Max, Max_1);

    try {
        TestSkipList(skip_list, in, out); ///The main test process
    }
    catch (std::bad_alloc&) { ///will exit if can not allocate more space
        cout << "no more available memory" << endl;
        in.close();
        out.close();
        return -1;
    }

    in.close();
    out.close();
    return 0;
}