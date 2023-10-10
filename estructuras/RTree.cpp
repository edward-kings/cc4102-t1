#include "RTree.hpp"
#include <iostream>
#include <math.h>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include "../algoritmos/RTreeAlgorithm.hpp"
#include "../disk_manipulation_functions/binaryFileFunctions.hpp"

RTree::RTree(int maxNodeCapacity, RTreeAlgorithm* algorithm, unsigned int numberOfRects) {
  this->maxNodeCapacity = maxNodeCapacity;
  this->algorithm = algorithm;
  this->numberOfRects = numberOfRects;
  this->totalSearchIOs = 0;
}

unsigned int RTree::getTotalSearchIOs() {
  return this->totalSearchIOs;
}

void RTree::resetTotalSearchIOs() {
  this->totalSearchIOs = 0;
}

void RTree::buildTreeFromFile(std::string filename) {
  unsigned int rootIndex = this->algorithm->buildTree(filename);
  this->treeFileBaseName = this->algorithm->getTreeFileBaseName();
  this->rootIndex = rootIndex;
}

bool RTree::intersects(Rect queryRegion, Rect MBR) {
  return queryRegion.x2 >= MBR.x1 && queryRegion.x1 <= MBR.x2 && queryRegion.y2 >= MBR.y1 && queryRegion.y1 <= MBR.y2;
}

std::vector<unsigned int> RTree::search(Rect region) {
  std::vector<unsigned int> result(0);
  this->searchRecursive(region,rootIndex,0,0,result);
  return result;
}

void RTree::searchRecursive(Rect region, int currentFileIndex, unsigned int firstChildIndex, unsigned int lastChildIndex, std::vector<unsigned int>& result) {
  if (currentFileIndex == -1) {
    std::ifstream leafFile("sortedRects.bin",std::ios::in|std::ios::binary);
    Rect* rects = new Rect[rectanglesPerBlock];
    binRectPageRead(leafFile,rects);
    this->totalSearchIOs++;
    int reads = 1;
    for (unsigned int i = firstChildIndex; i <= lastChildIndex; i+=sizeof(Rect)) {
      while (i / sizeof(Rect) >= rectanglesPerBlock * reads) {
        binRectPageRead(leafFile,rects);
        this->totalSearchIOs++;
        reads++;
      }
      Rect leaf;
      leaf = rects[(i / sizeof(Rect)) % rectanglesPerBlock];
      if (RTree::intersects(region,leaf)) {
        result.push_back(i);
      }
    }
    delete[] rects;
  } else {
    RTreeNode* nodes = new RTreeNode[nodesPerBlock];
    std::string filename = this->treeFileBaseName + std::to_string(currentFileIndex) + ".bin";
    std::ifstream treeFile(filename, std::ios::in | std::ios::binary);
    binNodePageRead(treeFile,nodes);
    this->totalSearchIOs++;
    int nodesRead = treeFile.gcount() / sizeof(RTreeNode);
    int end = lastChildIndex == 0 ? nodesRead : (int) std::ceil((double)lastChildIndex / sizeof(RTreeNode));
    for (int i = firstChildIndex / sizeof(RTreeNode); i < std::min(nodesRead,end); i++) {
        Rect MBR;
        MBR = nodes[i].MBR;
        if (RTree::intersects(region,MBR)) { 
          this->searchRecursive(region,currentFileIndex - 1, nodes[i].firstChildIndex, nodes[i].lastChildIndex,result);
        }
    }
    treeFile.close();
    delete[] nodes;
  }
}