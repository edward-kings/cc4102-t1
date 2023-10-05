#include <iostream>
#include <fstream>
#include "disk_manipulation_functions/generateRectangles.hpp"


int main() {
    std::fstream rectangulos;
    generateRectanglesFile(rectangulos,"rectangulos.bin",true, 1 << 10,3,false);
    std::fstream querys;
    generateRectanglesFile(querys,"querys.bin",true, 1 << 10, 2, true);
    return 0;
}
