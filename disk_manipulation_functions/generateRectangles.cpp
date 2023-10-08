#include <iostream>
#include "binaryFileFunctions.hpp"
#include <string.h>
using namespace std;

FILE* generateRectanglesFile(string filename, bool eraseContents, unsigned long amountOfRectangles, unsigned int seed, bool isSetQ) {
    FILE* rectFile = binOpen(filename,eraseContents);
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

        buffer[rectangleNumber].x1 = x1;
        buffer[rectangleNumber].x2 = x2;
        buffer[rectangleNumber].y1 = y1;
        buffer[rectangleNumber].y2 = y2;

        if (rectangleNumber == (rectanglesPerBlock - 1)) {
            unsigned int elements = binRectPageWrite(rectFile, buffer);
            cout << elements << endl;
            if (elements != rectanglesPerBlock) {
                if (feof(rectFile)) {
                    // Handle end of file error
                    std::cerr << "Error: End of file reached before all rectangles were written." << std::endl;
                } else {
                    // Handle mismatched element count error
                    std::cerr << "Error: Mismatched number of elements written to file." << std::endl;
                }
            }
        }
    }
    if (rectangleNumber != (rectanglesPerBlock - 1)) {
        unsigned int elements = binRectPageWrite(rectFile, buffer);
        if (elements != rectanglesPerBlock) {
            if (!feof(rectFile)) {
                    // Handle end of file error
                    std::cerr << "Error: End of file reached before all rectangles were written." << std::endl;
            }
        }
    }
    delete[] buffer;
    return rectFile;
}