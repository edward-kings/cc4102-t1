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
  unsigned int amountOfBytesToRead = lastChildIndex - firstChildIndex;
  if (currentFileIndex == -1) {
    std::ifstream leafFile("sortedRects.bin",std::ios::in|std::ios::binary);
    Rect* rects = new Rect[(amountOfBytesToRead / sizeof(Rect)) + 1];
    leafFile.seekg(firstChildIndex,std::ios::beg);
    binRectPageRead(leafFile,rects,amountOfBytesToRead / sizeof(Rect) + 1);
    this->totalSearchIOs+= std::ceil((double)amountOfBytesToRead / (double)diskBlockSize);
    unsigned int currentRectIndex = firstChildIndex;
    for (unsigned int i = 0; i <= amountOfBytesToRead / sizeof(Rect); i++) {
      Rect leaf;
      leaf = rects[i];
      if (RTree::intersects(region,leaf)) {
        result.push_back(currentRectIndex);
      }
      currentRectIndex+=sizeof(Rect);
    }
    delete[] rects;
  } else {
    RTreeNode* nodes = new RTreeNode[(amountOfBytesToRead / sizeof(RTreeNode)) + 1];
    std::string filename = this->treeFileBaseName + std::to_string(currentFileIndex) + ".bin";
    std::ifstream treeFile(filename, std::ios::in | std::ios::binary);
    treeFile.seekg(firstChildIndex,std::ios::beg);
    binNodePageRead(treeFile,nodes, (amountOfBytesToRead / sizeof(RTreeNode)) + 1);
    this->totalSearchIOs+= std::ceil((double)amountOfBytesToRead / (double)diskBlockSize);
    for (unsigned int i = 0; i <= amountOfBytesToRead / sizeof(RTreeNode); i++) {
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