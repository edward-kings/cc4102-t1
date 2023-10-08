#include <iostream>
#include <math.h>
#include "NearestXAlgorithm.hpp"

NearestXAlgorithm::NearestXAlgorithm(int maxNodeCapacity, unsigned long numberOfRects) {
  this->maxNodeCapacity = maxNodeCapacity;
  this->numberOfRects = numberOfRects;
}

std::vector<FILE*> NearestXAlgorithm::getTreeFiles() {
  return this->treeFiles;
}

void NearestXAlgorithm::setTreeFiles(std::vector<FILE*> &treeFiles) {
  this->treeFiles = treeFiles;
}

FILE*& NearestXAlgorithm::getSortedRectsFile() {
  return this->sortedRectsFile;
}

bool NearestXAlgorithm::orderCriteria(const Rect& rect1, const Rect& rect2) { 
  int rect1Center = floor(rect1.x1 + rect1.x2 / 2);
  int rect2Center = floor(rect2.x1 + rect2.x2 / 2);
  return rect1Center < rect2Center;
}

void NearestXAlgorithm::buildTree(FILE*& rectsFile) {
  this->externalMergeSort(rectsFile);
  fclose(rectsFile);
  binPageSeekFromStart(this->sortedRectsFile,0);
  Rect* rectBuffer = new Rect[rectanglesPerBlock];
  RTreeNode* nodeBuffer = new RTreeNode[nodesPerBlock];
  unsigned long amountOfNodes = ceil(numberOfRects / maxNodeCapacity);
  FILE* firstTreeFile = this->treeFiles[0];
  firstTreeFile = binOpen("NearestXRTree0.bin",true);
  for (unsigned long i = 0; i < amountOfNodes; i++) {
    RTreeNode node;
    node.firstChildIndex = binGetCurrentOffset(this->sortedRectsFile);
    binRectPageRead(this->sortedRectsFile,rectBuffer);
    node.lastChildIndex = binGetCurrentOffset(this->sortedRectsFile) - 1;
    node.MBR.x1 = rectBuffer[0].x1;
    node.MBR.y1 = rectBuffer[0].y1;
    node.MBR.x2 = rectBuffer[rectanglesPerBlock-1].x2;
    node.MBR.y2 = rectBuffer[rectanglesPerBlock-1].y2;
    unsigned int nodeNumber = i % nodesPerBlock;
    nodeBuffer[nodeNumber] = node;
    if (nodeNumber == nodesPerBlock - 1) {
      binNodePageWrite(firstTreeFile,nodeBuffer);
    }
  }
  delete[] rectBuffer;
  delete[] nodeBuffer;
  this->buildTreeRecursive(1, amountOfNodes);
}

void NearestXAlgorithm::buildTreeRecursive(unsigned long fileIndex, unsigned long currentNodeAmount) {
  if (currentNodeAmount == 1) return;
  FILE* previousTreeFile = this->treeFiles[fileIndex - 1];
  binPageSeekFromStart(previousTreeFile,0);

  RTreeNode* previousNodeBuffer = new RTreeNode[nodesPerBlock];
  RTreeNode* currentNodeBuffer = new RTreeNode[nodesPerBlock];
  unsigned long amountOfNodes = ceil(currentNodeAmount / maxNodeCapacity);
  FILE* currentTreeFile = this->treeFiles[fileIndex];
  currentTreeFile = binOpen("NearestXRTree" + std::to_string(fileIndex) + ".bin",true);
  for (unsigned long i = 0; i < amountOfNodes; i++) {
    RTreeNode node;
    node.firstChildIndex = binGetCurrentOffset(previousTreeFile);
    binNodePageRead(previousTreeFile,previousNodeBuffer);
    node.lastChildIndex = binGetCurrentOffset(previousTreeFile) - 1;
    node.MBR.x1 = previousNodeBuffer[0].MBR.x1;
    node.MBR.y1 = previousNodeBuffer[0].MBR.y1;
    node.MBR.x2 = previousNodeBuffer[node.lastChildIndex % nodesPerBlock].MBR.x2;
    node.MBR.y2 = previousNodeBuffer[node.lastChildIndex % nodesPerBlock].MBR.y2;
    unsigned long nodeNumber = i % nodesPerBlock;
    currentNodeBuffer[nodeNumber] = node;
    if (nodeNumber == nodesPerBlock - 1) {
      binNodePageWrite(currentTreeFile,currentNodeBuffer);
    }
  }
  fclose(previousTreeFile);
  delete[] previousNodeBuffer;
  delete[] currentNodeBuffer;
  this->buildTreeRecursive(fileIndex + 1, amountOfNodes);
}