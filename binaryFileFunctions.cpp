#include <fstream>
#include <iostream>
using namespace std;

void binIntWrite(fstream file, int value) {
    file.write(reinterpret_cast<char*>(&value), sizeof(value));
}

int binIntRead(fstream file) {
    int value;
    file.read(reinterpret_cast<char*>(&value),sizeof(value));
    return value;
}

fstream binOpen(string filename) {
    fstream file(filename,ios::out | ios::binary | ios::app);
    if (!file) {
        cerr << "Could not open the file!" << endl;
        exit(1);
    }
    return file;
}