#ifndef RTREE
#define RTREE

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "Rect.hpp"
#include "RTreeNode.hpp"
#include "../algoritmos/RTreeAlgorithm.hpp"
#include "../disk_manipulation_functions/binaryFileFunctions.hpp"


class RTree {
  public:
    RTree(int maxNodeCapacity, RTreeAlgorithm* algorithm, unsigned int numberOfRects);
    std::vector<unsigned int> search(Rect region);
    void buildTreeFromFile(std::string filename);
    unsigned int getTotalSearchIOs();
    void resetTotalSearchIOs();
    long rootIndex;
  private:
    unsigned int maxNodeCapacity;
    unsigned int numberOfRects;
    unsigned int totalSearchIOs;
    std::string treeFileBaseName;
    RTreeAlgorithm* algorithm;
    void searchRecursive(Rect region, int currentFileIndex, unsigned int firstChildIndex, unsigned int lastChildIndex, std::vector<unsigned int>& result);
    static bool intersects(Rect region, Rect MBR);
};


#endif