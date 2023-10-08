#include "RTreeAlgorithm.hpp"
#include <algorithm>
#include <vector>
#include <iostream>

unsigned int nodesPerBlock = diskBlockSize % sizeof(RTreeNode) == 0 ? 
                            diskBlockSize / sizeof(RTreeNode)  :
                            diskBlockSize / sizeof(RTreeNode)  - diskBlockSize % sizeof(RTreeNode);

void RTreeAlgorithm::externalMergeSort(FILE* &rectsFile) {
  // Step 1: Read the input file in chunks and sort each chunk

    Rect* buffer = new Rect[rectanglesPerBlock];
    int chunkNumber = 0;
    while (rectsFile) {
        long sizeRead = binRectPageRead(rectsFile,buffer);

        if (sizeRead > 0) {
            std::sort(buffer, buffer + sizeRead, [this](Rect& rect1, Rect& rect2) -> bool {return this->orderCriteria(rect1,rect2);});

            // Write each chunk to a temporary file
            FILE* chunk = binOpen("chunk_" + std::to_string(chunkNumber++) + ".bin",false);

            binRectPageWrite(chunk,buffer);
        }
    }

    // Step 2: Perform the external merge
    std::vector<FILE*> chunks(chunkNumber);

    for (int i = 0; i < chunkNumber; i++) {
        chunks[i] = binOpen("chunk_" + std::to_string(i) + ".bin",true);
    }

    FILE* sortedFile = this->getSortedRectsFile();

    std::vector<std::pair<Rect,int>> minHeap;

    // Populate the heap with the initial values from each chunk
    for (int i = 0; i < chunkNumber; i++) {
        Rect value;
        long dataRead = fread(&value,sizeof(Rect),1,chunks[i]);
        if (dataRead == sizeof(Rect)) {
            minHeap.push_back(std::pair(value,i));
        } else if (ferror(chunks[i])) {
            std::cerr << "Error reading from temporary chunk file " + std::to_string(i) << std::endl;
            exit(1);
        }
    }

    std::make_heap(minHeap.begin(), minHeap.end(), [this](std::pair<Rect,int>& rect1, std::pair<Rect,int> rect2) -> bool {return this->orderCriteria(rect1.first,rect2.first);});

    // Merge step
    while (!minHeap.empty()) {
        // Get the minimum value from the heap
        std::pair<Rect,int> minValue = minHeap.front();
        std::pop_heap(minHeap.begin(), minHeap.end(), [this](std::pair<Rect,int>& rect1, std::pair<Rect,int> rect2) -> bool {return this->orderCriteria(rect1.first,rect2.first);});
        minHeap.pop_back();
        int chunkIndex = minValue.second;
        // Write the minimum value to the output file
        fwrite(&minValue,sizeof(Rect),1,sortedFile);

        // Read the next value from the same chunk
        Rect nextValue;
        if (fread(&nextValue,sizeof(Rect),1,chunks[chunkIndex])) {
            // If there's a next value, add it to the heap
            minHeap.push_back(std::pair(nextValue,chunkIndex));
            std::push_heap(minHeap.begin(), minHeap.end(), [this](std::pair<Rect,int>& rect1, std::pair<Rect,int> rect2) -> bool {return this->orderCriteria(rect1.first,rect2.first);});
        }
    }

    for (auto& chunk : chunks) {
        fclose(chunk);
    }

    // Clean up temporary chunk files
    for (int i = 0; i < chunkNumber; i++) {
        std::remove(("chunk_" + std::to_string(i) + ".bin").c_str());
    }
    delete[] buffer;
};