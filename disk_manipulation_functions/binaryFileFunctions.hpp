#ifndef BINARY_FILE_FUNCTIONS
#define BINARY_FILE_FUNCTIONS

#include <cstdio>
#include <string>
#include "../estructuras/Rect.hpp"
#include "../estructuras/RTreeNode.hpp"
#include "../algoritmos/RTreeAlgorithm.hpp"

#ifdef _WIN32
#include <Windows.h>
#else
#include <sys/statvfs.h>
#endif

unsigned int getDiskBlockSize();
extern unsigned int diskBlockSize;
extern unsigned int rectanglesPerBlock;
extern unsigned int nodesPerBlock;
unsigned int binRectPageWrite(std::ofstream &file, Rect* buffer, unsigned int rectanglesToWrite = rectanglesPerBlock);
unsigned int binRectPageRead(std::ifstream &file, Rect* buffer, unsigned int rectanglesToRead = rectanglesPerBlock);
unsigned int binNodePageWrite(std::ofstream &file, RTreeNode* buffer, unsigned int nodesToWrite = nodesPerBlock);
unsigned int binNodePageRead(std::ifstream &file, RTreeNode* buffer, unsigned int nodesToRead = nodesPerBlock);


#endif
