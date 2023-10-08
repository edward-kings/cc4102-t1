#include "RTree.hpp"

RTree::RTree(int maxNodeCapacity, RTreeAlgorithm* algorithm, unsigned long numberOfRects) {
  this->maxNodeCapacity = maxNodeCapacity;
  this->algorithm = algorithm;
  this->numberOfRects = numberOfRects;
  this->treeFiles = std::vector<FILE*>();
  algorithm->setTreeFiles(this->treeFiles);
}

void RTree::buildTreeFromFile(FILE* &rectsFile) {
  binPageSeekFromStart(rectsFile,0);
  this->algorithm->buildTree(rectsFile);
}