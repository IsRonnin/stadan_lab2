#include "TList.h"

#include <fstream>
#include <iostream>

using namespace std;

int main() {
    ifstream input("input.txt");
    StudentList students;
    if (!students.read(input)) {
        cerr << "Cannot read input.txt\n";
        return 1;
    }
    students.processSession(cout);
    cout << "\nИтоговый список\n";
    students.printList(cout);
}
