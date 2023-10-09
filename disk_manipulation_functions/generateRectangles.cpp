#include <iostream>
#include "binaryFileFunctions.hpp"
#include <string.h>
#include <random>
#include <fstream>
using namespace std;

void generateRectanglesFile(string filename, bool eraseContents, unsigned int amountOfRectangles, unsigned int seed, bool isSetQ) {
    std::ios::openmode mode = eraseContents ? std::ios::trunc : std::ios::app;
    std::ofstream rectFile(filename, std::ios::out | std::ios::binary | mode);
    std::random_device rd;
    std::mt19937 gen(rd());
    gen.seed(seed);
    std::uniform_int_distribution<unsigned int> distrib(0U, 500000U);
    Rect* buffer = new Rect[rectanglesPerBlock];
    unsigned int rectangleNumber = 0;
    for (unsigned int i = 0; i < amountOfRectangles; i++) {
        int x1,y1,x2,y2;
        int maxSideSize = isSetQ ? 100001U : 101U;

        x1 = distrib(gen);
        y1 = distrib(gen);
        x2 = min(x1 + (distrib(gen) % maxSideSize),500000U); 
        y2 = min(y1 + (distrib(gen) % maxSideSize),500000U);

        rectangleNumber = i % rectanglesPerBlock;

        buffer[rectangleNumber].x1 = x1;
        buffer[rectangleNumber].x2 = x2;
        buffer[rectangleNumber].y1 = y1;
        buffer[rectangleNumber].y2 = y2;

        if (rectangleNumber == (rectanglesPerBlock - 1)) {
            rectFile.write(reinterpret_cast<char*>(buffer), sizeof(Rect) * rectanglesPerBlock);
        }
    }
    if (rectangleNumber != (rectanglesPerBlock - 1)) {
            rectFile.write(reinterpret_cast<char*>(buffer), sizeof(Rect) * (rectangleNumber + 1));
    }
    delete[] buffer;
    rectFile.close();
}
