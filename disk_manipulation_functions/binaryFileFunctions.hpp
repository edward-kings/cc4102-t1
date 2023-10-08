#ifndef BINARY_FILE_FUNCTIONS
#define BINARY_FILE_FUNCTIONS

#include <cstdio>
#include <string>
#include "../estructuras/Rect.hpp"
#include "../estructuras/RTreeNode.hpp"

#ifdef _WIN32
#include <Windows.h>
#else
#include <sys/statvfs.h>
#endif

unsigned long getDiskBlockSize();
extern unsigned long diskBlockSize;
extern unsigned int rectanglesPerBlock;
unsigned int binRectPageWrite(FILE* &file, Rect* buffer);
unsigned int binRectPageRead(FILE* &file, Rect* buffer);
unsigned int binNodePageWrite(FILE* &file, RTreeNode* buffer);
unsigned int binNodePageRead(FILE* &file, RTreeNode* buffer);
FILE* binOpen(std::string filename, bool eraseContents);
unsigned long binGetCurrentOffset(FILE* &file);
unsigned int binPageSeekFromStart(FILE* &file, long offset);


#endif
