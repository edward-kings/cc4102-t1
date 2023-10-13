#include <cstdio>
#include <iostream>
#include <string>
#include <fstream>
#include "../estructuras/Rect.hpp"
#include "../estructuras/RTreeNode.hpp"
#include "../algoritmos/RTreeAlgorithm.hpp"
#ifdef _WIN32
#include <Windows.h>
#else
#include <sys/statvfs.h>
#endif


unsigned int getDiskBlockSize() {
#ifdef _WIN32
    DWORD sectorsPerCluster, bytesPerSector, freeClusters, totalClusters;

    // Convert narrow-character string to wide-character string

    if (GetDiskFreeSpace(".", &sectorsPerCluster, &bytesPerSector, &freeClusters, &totalClusters)) {
        return bytesPerSector * sectorsPerCluster;
    } else {
        // Handle error
        return -1;
    }
#else
    struct statvfs info;
    int success = statvfs(".", &info);
    if (success != 0) {
        return -1;
    } else {
        return info.f_bsize;
    }
#endif
}


unsigned int diskBlockSize = getDiskBlockSize();
unsigned int rectanglesOverLimit = diskBlockSize % sizeof(Rect);

unsigned int rectanglesPerBlock = rectanglesOverLimit == 0 ?
diskBlockSize / sizeof(Rect) :
    diskBlockSize / sizeof(Rect) - rectanglesOverLimit;

unsigned int nodesPerBlock = diskBlockSize % sizeof(RTreeNode) == 0 ?
diskBlockSize / sizeof(RTreeNode) :
    diskBlockSize / sizeof(RTreeNode) - diskBlockSize % sizeof(RTreeNode);

unsigned int binRectPageWrite(std::ofstream& file, Rect* buffer, unsigned int rectanglesToWrite) {
    unsigned int seekBefore = file.tellp();
    file.write(reinterpret_cast<char*>(buffer), sizeof(Rect) * rectanglesToWrite);
    return (unsigned int)file.tellp() - seekBefore;
}

unsigned  int binNodePageWrite(std::ofstream& file, RTreeNode* buffer, unsigned int nodesToWrite) {
    unsigned int seekBefore = file.tellp();
    file.write(reinterpret_cast<char*>(buffer), sizeof(RTreeNode) * nodesToWrite);
    return (unsigned int)file.tellp() - seekBefore;
}


unsigned int binRectPageRead(std::ifstream& file, Rect* buffer, unsigned int rectanglesToRead) {
    file.read(reinterpret_cast<char*>(buffer), sizeof(Rect) * rectanglesToRead);
    return (unsigned int)file.gcount();
}

unsigned int binNodePageRead(std::ifstream& file, RTreeNode* buffer, unsigned int nodesToRead) {
    file.read(reinterpret_cast<char*>(buffer), sizeof(RTreeNode) * nodesToRead);
    return (unsigned int)file.gcount();
}
