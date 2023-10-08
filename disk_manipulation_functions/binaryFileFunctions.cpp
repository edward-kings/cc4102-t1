#include <cstdio>
#include <iostream>
#include <string>
#include "../estructuras/Rect.hpp"
#include "../estructuras/RTreeNode.hpp"
#include "../algoritmos/RTreeAlgorithm.hpp"
#ifdef _WIN32
#include <Windows.h>
#else
#include <sys/statvfs.h>
#endif


unsigned long getDiskBlockSize() {
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
    int success = statvfs(".",&info);
    if (success != 0) {
        return -1;
    } else {
        return info.f_bsize;
    }
#endif
}


unsigned long diskBlockSize = getDiskBlockSize();
unsigned int rectanglesOverLimit = diskBlockSize % sizeof(Rect);

unsigned int rectanglesPerBlock = rectanglesOverLimit == 0 ? 
                            diskBlockSize / sizeof(Rect)  :
                            diskBlockSize / sizeof(Rect)  - rectanglesOverLimit;

unsigned int binRectPageWrite(FILE* &file, Rect* buffer) {
    return fwrite(buffer,sizeof(Rect),rectanglesPerBlock,file);
}

unsigned  int binNodePageWrite(FILE* &file, RTreeNode* buffer) {
    return fwrite(buffer,sizeof(RTreeNode),nodesPerBlock,file);
}


unsigned  int binPageSeekFromStart(FILE* &file, long offset) {
    return fseek(file,offset,SEEK_SET);
}

unsigned long binGetCurrentOffset(FILE* &file) {
    return ftell(file);
}

unsigned int binRectPageRead(FILE* &file, Rect* buffer) {
    return fread(buffer,sizeof(Rect),rectanglesPerBlock,file);
}

unsigned int binNodePageRead(FILE* &file, RTreeNode* buffer) {
    return fread(buffer,sizeof(RTreeNode),nodesPerBlock,file);
}

FILE* binOpen(std::string filename, bool eraseContents) {
    const char* mode = eraseContents ? "wb+" : "ab+";
    FILE* file = new FILE;
    file = fopen(filename.c_str(),mode);
    if (!file) {
        std::cerr << "Could not open the file!" << std::endl;
        exit(1);
    }
    return file;
}
