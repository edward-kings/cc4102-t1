#include <fstream>
#include <iostream>
#include "../Rect.hpp"

#ifdef _WIN32
#include <Windows.h>
#else
#include <sys/statvfs.h>
#endif

unsigned long getDiskBlockSize();
extern unsigned long diskBlockSize;
extern unsigned int rectanglesPerBlock;
bool binPageWrite(std::fstream &file, Rect* buffer);
bool binPageRead(std::fstream &file, Rect* buffer);
void binOpen(std::string filename, bool eraseContents, std::fstream &file);
