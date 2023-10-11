#include <queue>
#include <fstream>
#include <algorithm>
#include <vector>
#include <iostream>
#include <string>
#include "RTreeAlgorithm.hpp"
#include "../estructuras/Rect.hpp"
#include "../disk_manipulation_functions/binaryFileFunctions.hpp"
#include "../estructuras/RTreeNode.hpp"

void RTreeAlgorithm::externalMergeSort(std::string filename) {
  // Step 1: Read the input file in chunks and sort each chunk
    std::ifstream rectsFile(filename, std::ios::in | std::ios::binary);
    Rect* buffer = new Rect[rectanglesPerBlock];
    int chunkNumber = 0;
    unsigned int chunkSize = 0;
    std::ofstream chunk("chunk_" + std::to_string(chunkNumber) + ".bin", std::ios::out | std::ios::binary | std::ios::trunc);
    while (rectsFile) {
        long sizeRead = binRectPageRead(rectsFile,buffer) / sizeof(Rect);
        while (sizeRead > 0) {
            std::sort(buffer, buffer + sizeRead, [this](Rect& rect1, Rect& rect2) -> bool {return this->orderCriteria(rect1,rect2);});

            // Write each chunk to a temporary file
            
            binRectPageWrite(chunk,buffer);
            chunkSize += sizeRead;
            if (chunkSize >= (diskBlockSize * 3000) / sizeof(Rect)) {
                chunkNumber++;
                chunkSize = 0;
                chunk.close();
                chunk = std::ofstream("chunk_" + std::to_string(chunkNumber) + ".bin", std::ios::out | std::ios::binary | std::ios::trunc);
            }
            sizeRead = binRectPageRead(rectsFile,buffer) / sizeof(Rect);
        }
        if (sizeRead == 0) break;
    }
    if (chunk.is_open()) {
        chunkNumber++;
        chunk.close();
    }
    // Step 2: Perform the external merge
    std::vector<std::ifstream> chunks(chunkNumber);

    for (int i = 0; i < chunkNumber; i++) {
        chunks[i] = std::ifstream("chunk_" + std::to_string(i) + ".bin", std::ios::in | std::ios::binary);
    }


    

    auto orderLambda = [this](std::pair<Rect,int>& rect1, std::pair<Rect,int>& rect2) -> bool {return !this->orderCriteria(rect1.first,rect2.first);};
    std::priority_queue<std::pair<Rect,int>,std::vector<std::pair<Rect,int>>,decltype(orderLambda)> minHeap(orderLambda);

    // Populate the heap with the initial values from each chunk
    for (int i = 0; i < chunkNumber; i++) {
        Rect value;
        chunks[i].read(reinterpret_cast<char*>(&value),sizeof(Rect));
        auto dataRead = chunks[i].gcount();
        if (dataRead == sizeof(Rect)) {
            minHeap.push(std::pair(value,i));
        } else if (chunks[i].fail()) {
            std::cerr << "Error reading from temporary chunk file " + std::to_string(i) << std::endl;
            exit(1);
        }
    }

    std::ofstream sortedFile(this->getSortedRectsFileName(),std::ios::out | std::ios::binary);
    // Merge step
    while (!minHeap.empty()) {
        // Get the minimum value from the heap
        std::pair<Rect,int> minValue = minHeap.top();
        minHeap.pop();
        int chunkIndex = minValue.second;
        // Write the minimum value to the output file
        sortedFile.write(reinterpret_cast<char*>(&minValue.first),sizeof(Rect));

        // Read the next value from the same chunk
        Rect nextValue;
        chunks[chunkIndex].read(reinterpret_cast<char*>(&nextValue),sizeof(Rect));
        if (chunks[chunkIndex].gcount() == sizeof(Rect)) {
            // If there's a next value, add it to the heap
            minHeap.push(std::pair(nextValue,chunkIndex));
        }
    }

    for (auto& chunk : chunks) {
        chunk.close();
    }

    // Clean up temporary chunk files
    for (int i = 0; i < chunkNumber; i++) {
        std::remove(("chunk_" + std::to_string(i) + ".bin").c_str());
    }
    delete[] buffer;
    sortedFile.close();
};