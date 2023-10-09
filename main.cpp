#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include "./estructuras/RTree.hpp"
#include "./algoritmos/NearestXAlgorithm.hpp"
#include "./disk_manipulation_functions/generateRectangles.hpp"

int main() {
    std::ofstream outPutFile("output.csv");
    unsigned int totalIoSum = 0;
    for (unsigned int k = 10; k < 26; k++) {
        unsigned int ioSum = 0;
        auto totalDuration = std::chrono::microseconds::zero();
        unsigned int numberOfRects = 1 << k;
        RTreeAlgorithm* nearestX = new NearestXAlgorithm(nodesPerBlock, numberOfRects);
        RTree* tree = new RTree(nodesPerBlock/2, nearestX, numberOfRects);
        generateRectanglesFile("rects.bin",true,numberOfRects,1,false);
        std::ofstream results("results" + std::to_string(k) + ".txt", std::ios::out);
        tree->buildTreeFromFile("rects.bin"	);
        generateRectanglesFile("querys.bin",true,100,4,true);
        Rect* querys = new Rect[100];
        std::ifstream querysFile("querys.bin",std::ios::in|std::ios::binary);
        binRectPageRead(querysFile,querys,100);
        querysFile.close();
        std::stringstream output;
        output << "Queries n = 2^" << k << " ; Método Nearest X" << "\n";
        for (int i = 0; i < 100; i++) {
            Rect query = querys[i];
            tree->resetTotalSearchIOs();
            output << "Rect (" << query.x1 << "," << query.y1 << "," << query.x2 << "," << query.y2 << ");";
            auto start = std::chrono::high_resolution_clock::now();
            std::vector<unsigned int> result = tree->search(query);
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end-start);
            totalDuration += duration;
            ioSum += tree->getTotalSearchIOs();
            totalIoSum += ioSum;
            output << duration.count() << ";" << tree->getTotalSearchIOs() << "\n";
            results << "Query" << i << ": " << query.x1 << " " << query.y1 << " " << query.x2 << " " << query.y2 << std::endl << std::endl;
            if (result.size() > 0) {
                std::ifstream leafFile("sortedRects.bin",std::ios::in|std::ios::binary);
                Rect* rects = new Rect[rectanglesPerBlock];
                binRectPageRead(leafFile,rects);
                int reads = 1;
                for (unsigned int j = 0; j < result.size(); j++) {
                    while (result[j] >= rectanglesPerBlock * reads) {
                        binRectPageRead(leafFile,rects);
                        reads++;
                    }
                    results << "Leaf at byte " << result[j] << ": " << std::endl;    
                    results << "Rect: (" << rects[result[j] % rectanglesPerBlock].x1 << "," << rects[result[j] % rectanglesPerBlock].y1 << "," << rects[result[j] % rectanglesPerBlock].x2 << "," << rects[result[j] % rectanglesPerBlock].y2 << ")" << std::endl;
                }
                leafFile.close();
                delete[] rects;
            } else {
                results << "No results for this query" << std::endl;
            }
            results << std::endl;
        }
        output << "; " << totalDuration.count() << ";" << ioSum << "\n\n";
        totalDuration = std::chrono::microseconds::zero();
        ioSum = 0;
        results.close();
        outPutFile << output.str();
    }
    std::cout << "Total IOs: " << totalIoSum << std::endl;
    outPutFile.close();
    return 0;
}
