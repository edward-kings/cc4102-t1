#include <fstream>
#include <iostream>
#include "../Rect.hpp"
#ifdef _WIN32
#include <Windows.h>
#else
#include <sys/statvfs.h>
#endif

using namespace std;


unsigned long getDiskBlockSize() {
#ifdef _WIN32
    DWORD sectorsPerCluster, bytesPerSector, freeClusters, totalClusters;

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

bool binPageWrite(fstream &file, Rect* buffer) {
    file.write(reinterpret_cast<char*>(buffer), sizeof(Rect) * rectanglesPerBlock);
    return file.fail() ? true : false;
}

bool binPageRead(fstream &file, Rect* buffer) {
    file.read(reinterpret_cast<char*>(buffer),sizeof(Rect) * rectanglesPerBlock);
    return file.fail() ? true : false;
}

void binOpen(string filename, bool eraseContents, fstream &file) {
    ios::openmode mode = eraseContents ? ios::trunc : ios::app;
    file.open(filename,ios::in | ios::out |ios::binary | mode);
    if (!file) {
        cerr << "Could not open the file!" << endl;
        exit(1);
    }
}
