#include <iostream>
#include <fstream>
#include "disk_manipulation_functions/generateRectangles.hpp"


int main(int argc, char** argv) {
    int seed1 = 3;
    int seed2 = 4;
    int amount = 1 << 10;
    if (argc >= 3) {
        seed1 = atoi(argv[1]);
        seed2 = atoi(argv[2]);
        amount = argc == 4 ? atoi(argv[3]) : amount;
    }
    std::fstream rectangulos;
    generateRectanglesFile(rectangulos,"rectangulos.bin",true, amount,seed1,false);
    std::fstream querys;
    generateRectanglesFile(querys,"querys.bin",true, 100, seed2, true);
    rectangulos.close();
    querys.close();
    return 0;
}
