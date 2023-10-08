#ifndef RTREE
#define RTREE

#include <iostream>
#include <fstream>
#include <vector>
#include "Rect.hpp"
#include "RTreeNode.hpp"
#include "../algoritmos/RTreeAlgorithm.hpp"
#include "../disk_manipulation_functions/binaryFileFunctions.hpp"


class RTree {
  public:
    RTree(int maxNodeCapacity, RTreeAlgorithm* algorithm, unsigned long numberOfRects);
    RTree& search(Rect* region);
    void buildTreeFromFile(FILE* &rectsFile);
    long pointerToRoot;
  private:
    unsigned long maxNodeCapacity;
    unsigned long numberOfRects;
    std::vector<FILE*> treeFiles;
    RTreeAlgorithm* algorithm;
};


#endif