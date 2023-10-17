#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include "./estructuras/RTree.hpp"
#include "./algoritmos/NearestXAlgorithm.hpp"
#include "./algoritmos/HilbertAlgorithm.hpp"
#include "./algoritmos/SortTileRecursiveAlgorithm.hpp"
#include "./disk_manipulation_functions/generateRectangles.hpp"

int main(int argc, char** argv) {
    int seedRect = 0;
    int seedQuery = 0;
    if (argc == 3) {
        seedRect = std::stoi(argv[1]);
        seedQuery = std::stoi(argv[2]);
    }
    std::ofstream outPutFile("output.csv");
    unsigned int totalIoSumNearest = 0;
    unsigned int totalIoSumHilbert = 0;
    unsigned int totalioSumStr = 0;
    for (unsigned int k = 10; k < 26; k++) {
        unsigned int ioSumNearest = 0;
        unsigned int ioSumHilbert = 0;
        unsigned int ioSumStr = 0;
        auto totalDurationNearest = std::chrono::microseconds::zero();
        auto totalDurationHilbert = std::chrono::microseconds::zero();
        auto totalDurationStr = std::chrono::microseconds::zero();
        unsigned int numberOfRects = 1 << k;
        RTreeAlgorithm* nearestX = new NearestXAlgorithm(nodesPerBlock, numberOfRects);
        RTreeAlgorithm* hilbert = new HilbertAlgorithm(nodesPerBlock, numberOfRects);
        RTreeAlgorithm* str = new SortTileRecursiveAlgorithm(nodesPerBlock, numberOfRects);
        RTree* nearestTree = new RTree(nodesPerBlock, nearestX, numberOfRects);
        RTree* hilbertTree = new RTree(nodesPerBlock, hilbert, numberOfRects);
        RTree* strTree = new RTree(nodesPerBlock, str, numberOfRects);
        generateRectanglesFile("rects.bin", true, numberOfRects, seedRect, false);
        std::ofstream txtNearest("resultsNearest" + std::to_string(k) + ".txt", std::ios::out);
        std::ofstream txtHilbert("resultsHilbert" + std::to_string(k) + ".txt", std::ios::out);
        std::ofstream txtStr("resultsStr" + std::to_string(k) + ".txt", std::ios::out);
        nearestTree->buildTreeFromFile("rects.bin");
        hilbertTree->buildTreeFromFile("rects.bin");
        strTree->buildTreeFromFile("rects.bin");
        generateRectanglesFile("querys.bin", true, 100, seedQuery, true);
        Rect* querys = new Rect[100];
        std::ifstream querysFile("querys.bin", std::ios::in | std::ios::binary);
        binRectPageRead(querysFile, querys, 100);
        querysFile.close();
        std::stringstream output;
        output << "Queries n = 2^" << k << " ; Tiempo Nearest X; IO/s Nearest X ; Tiempo Hilbert ; IO/s Hilbert ; Tiempo STR; IO/s STR " << "\n";
        for (int i = 0; i < 100; i++) {
            Rect query = querys[i];
            nearestTree->resetTotalSearchIOs();
            hilbertTree->resetTotalSearchIOs();
            strTree->resetTotalSearchIOs();
            output << "Rect (" << query.x1 << "," << query.y1 << "," << query.x2 << "," << query.y2 << ");";
            system("echo 3 > /proc/sys/vm/drop_caches");
            auto start = std::chrono::high_resolution_clock::now();
            std::vector<Rect> resultNearest = nearestTree->search(query);
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            totalDurationNearest += duration;
            ioSumNearest += nearestTree->getTotalSearchIOs();
            totalIoSumNearest += ioSumNearest;
            output << duration.count() << ";" << nearestTree->getTotalSearchIOs() << ";";
            txtNearest << "Query" << i << ": " << query.x1 << " " << query.y1 << " " << query.x2 << " " << query.y2 << std::endl << std::endl;
            if (resultNearest.size() > 0) {
                for (unsigned int j = 0; j < resultNearest.size(); j++) {
                    txtNearest << "Rect: (" << resultNearest[j].x1 << "," << resultNearest[j].y1 << "," << resultNearest[j].x2 << "," << resultNearest[j].y2 << ")" << std::endl;
                }
            } else {
                txtNearest << "No results for this query" << std::endl;
            }
            txtNearest << std::endl;
            system("echo 3 > /proc/sys/vm/drop_caches");
            start = std::chrono::high_resolution_clock::now();
            std::vector<Rect> resultHilbert = hilbertTree->search(query);
            end = std::chrono::high_resolution_clock::now();
            duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            totalDurationHilbert += duration;
            ioSumHilbert += hilbertTree->getTotalSearchIOs();
            totalIoSumHilbert += ioSumHilbert;
            output << duration.count() << ";" << hilbertTree->getTotalSearchIOs() << ";";
            txtHilbert << "Query" << i << ": " << query.x1 << " " << query.y1 << " " << query.x2 << " " << query.y2 << std::endl << std::endl;
            if (resultHilbert.size() > 0) {
                for (unsigned int j = 0; j < resultHilbert.size(); j++) {
                    txtHilbert << "Rect: (" << resultHilbert[j].x1 << "," << resultHilbert[j].y1 << "," << resultHilbert[j].x2 << "," << resultHilbert[j].y2 << ")" << std::endl;
                }
            } else {
                txtHilbert << "No results for this query" << std::endl;
            }
            txtHilbert << std::endl;
            system("echo 3 > /proc/sys/vm/drop_caches");
            start = std::chrono::high_resolution_clock::now();
            std::vector<Rect> resultStr = strTree->search(query);
            end = std::chrono::high_resolution_clock::now();
            duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            totalDurationStr += duration;
            ioSumStr += strTree->getTotalSearchIOs();
            totalioSumStr += ioSumStr;
            output << duration.count() << ";" << strTree->getTotalSearchIOs() << "\n";
            txtStr << "Query" << i << ": " << query.x1 << " " << query.y1 << " " << query.x2 << " " << query.y2 << std::endl << std::endl;
            if (resultStr.size() > 0) {
                for (unsigned int j = 0; j < resultStr.size(); j++) {
                    txtStr << "Rect: (" << resultStr[j].x1 << "," << resultStr[j].y1 << "," << resultStr[j].x2 << "," << resultStr[j].y2 << ")" << std::endl;
                }
            } else {
                txtStr << "No results for this query" << std::endl;
            }
            txtStr << std::endl;
        }
        output << "; " << totalDurationNearest.count() << ";" << ioSumNearest << ";" << totalDurationHilbert.count() << ";" << ioSumHilbert << ";" << totalDurationStr.count() << ";" << ioSumStr << "\n\n";
        totalDurationNearest = std::chrono::microseconds::zero();
        totalDurationHilbert = std::chrono::microseconds::zero();
        totalDurationStr = std::chrono::microseconds::zero();
        ioSumNearest = 0;
        ioSumHilbert = 0;
        ioSumStr = 0;
        txtNearest.close();
        txtHilbert.close();
        outPutFile << output.str();
        delete[] querys;
        delete nearestTree;
        delete nearestX;
        delete hilbertTree;
        delete hilbert;
        delete strTree;
        delete str;
    }
    std::cout << "Total IOs: " << totalIoSumNearest << std::endl;
    outPutFile.close();
    return 0;
}
