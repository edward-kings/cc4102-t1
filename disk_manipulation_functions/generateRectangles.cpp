#include <iostream>
#include <fstream>
#include "binaryFileFunctions.hpp"
#include <string.h>
using namespace std;

void generateRectanglesFile(fstream &rectFile,string filename, bool eraseContents, unsigned long amountOfRectangles, unsigned int seed, bool isSetQ) {
    binOpen(filename,eraseContents,rectFile);
    srand(seed);
    Rect* buffer = new Rect[rectanglesPerBlock];
    unsigned int rectangleNumber;
    for (unsigned long i = 0; i < amountOfRectangles; i++) {
        int x1,y1,x2,y2;
        int maxSideSize = isSetQ ? 100001 : 101;

        x1 = rand() % 500001;
        y1 = rand() % 500001;
        x2 = min(x1 + (rand() % maxSideSize),500000); 
        y2 = min(y1 + (rand() % maxSideSize),500000);

        rectangleNumber = i % rectanglesPerBlock;
        if (i < rectanglesPerBlock) {
            cout << "Rect: (" << x1 << "," << y1 << "," << x2 << "," << y2 << ")" <<endl;
        }
        buffer[rectangleNumber].x1 = x1;
        buffer[rectangleNumber].x2 = x2;
        buffer[rectangleNumber].y1 = y1;
        buffer[rectangleNumber].y2 = y2;

        if (rectangleNumber == (rectanglesPerBlock - 1)) {
            bool failure = binPageWrite(rectFile, buffer);
            if (failure) {
                cerr << "Couldn't write a rectangle: " << strerror(errno) << endl;
                exit(1);
            }
        }
    }
    if (rectangleNumber != (rectanglesPerBlock - 1)) {
        bool failure = binPageWrite(rectFile, buffer);
        if (failure) {
                cerr << "Couldn't write a rectangle: " << strerror(errno) << endl;
                exit(1);
        }
    }
}