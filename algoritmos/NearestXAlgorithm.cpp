#include <iostream>
#include <fstream>
#include <cstring>
#include <math.h>
#include "NearestXAlgorithm.hpp"

NearestXAlgorithm::NearestXAlgorithm(int maxNodeCapacity, unsigned int numberOfRects) {
  this->maxNodeCapacity = maxNodeCapacity;
  this->numberOfRects = numberOfRects;
  this->sortedRectsFileName = "sortedRects.bin";
  this->treeFileBaseName = "NearestXRTree";
}

void NearestXAlgorithm::setSortedRectsFileName(std::string sortedRectsFileName) {
  this->sortedRectsFileName = sortedRectsFileName;
}

std::string NearestXAlgorithm::getTreeFileBaseName() {
  return this->treeFileBaseName;
}

int NearestXAlgorithm::getNumberOfRects() {
  return this->numberOfRects;
}

int NearestXAlgorithm::getMaxNodeCapacity() {
  return this->maxNodeCapacity;
}


std::string NearestXAlgorithm::getSortedRectsFileName() {
  return this->sortedRectsFileName;
}

bool NearestXAlgorithm::orderCriteria(const Rect& rect1, const Rect& rect2) { 
  int rect1Center = floor(rect1.x1 + rect1.x2 / 2);
  int rect2Center = floor(rect2.x1 + rect2.x2 / 2);
  return rect1Center < rect2Center;
}

unsigned int NearestXAlgorithm::buildTree(std::string filename) {
  this->externalMergeSort(filename);
  
  std::ifstream sortedRects(this->sortedRectsFileName,std::ios::in | std::ios::binary);
  Rect* rectBuffer = new Rect[rectanglesPerBlock];
  RTreeNode* nodeBuffer = new RTreeNode[nodesPerBlock];
  // amountOfNodes = n/M the maximum amount of internal nodes that can be created from n rectangles at this level
  unsigned int amountOfNodes = std::ceil((double)numberOfRects / this->maxNodeCapacity);
  std::string currentFilename = "NearestXRTree0.bin";
  std::ofstream firstTreeFile("NearestXRTree0.bin",std::ios::out | std::ios::binary | std::ios::in | std::ios::trunc);
  unsigned int nodeNumber = 0;
  unsigned int blockLimit = 0, lastChildIndex = 0;
  unsigned int firstChildIndex = 0;
  int previousRectx1 = 0, previousRecty1 = 0;
  bool bufferOverflow = false;
  while (nodeNumber < amountOfNodes) {
    binRectPageRead(sortedRects,rectBuffer);
    int dataRead = sortedRects.gcount();
    int currentPos = sortedRects.tellg();
    // If the current position is -1, we are at the end of the file
    if (currentPos == -1) {
      if (sortedRects.eof()) {
        // And the limit will be based on the amount of data on the last read or whatever was the limit before
        blockLimit = lastChildIndex == 0 ? dataRead - sizeof(Rect) : std::min(blockLimit,lastChildIndex);
      }
    } else {
        blockLimit = currentPos - (unsigned int) sizeof(Rect);
    }
    lastChildIndex = std::min((unsigned int) (firstChildIndex + (this->maxNodeCapacity - 1) * sizeof(Rect)),blockLimit);
    while (lastChildIndex <= blockLimit) {
      RTreeNode node;
      node.firstChildIndex = firstChildIndex;
      node.lastChildIndex = lastChildIndex;
      // The first index in the buffer is the conversion of the first child index's size in bytes to the size of a Rect
      unsigned int firstIndexInBuf = (firstChildIndex / sizeof(Rect)) % rectanglesPerBlock;
      unsigned int lastIndexInBuf = (lastChildIndex / sizeof(Rect)) % rectanglesPerBlock;
      // If the first index was greater than the last index on the previous iteration, we need to use the previous value of the first index
      if (bufferOverflow) {
        node.MBR.x1 = previousRectx1;
        node.MBR.y1 = previousRecty1;
        node.MBR.x2 = rectBuffer[lastIndexInBuf].x2;
        node.MBR.y2 = rectBuffer[lastIndexInBuf].y2;
        bufferOverflow = false;
      } else {
        node.MBR.x1 = rectBuffer[firstIndexInBuf].x1;
        node.MBR.y1 = rectBuffer[firstIndexInBuf].y1;
        node.MBR.x2 = rectBuffer[lastIndexInBuf].x2;
        node.MBR.y2 = rectBuffer[lastIndexInBuf].y2;
      }
      nodeBuffer[nodeNumber] = node;
      nodeNumber++;
      firstChildIndex = lastChildIndex + sizeof(Rect);
      lastChildIndex = firstChildIndex + (this->maxNodeCapacity - 1) * sizeof(Rect);
      firstIndexInBuf = (firstChildIndex / sizeof(Rect)) % rectanglesPerBlock;
      lastIndexInBuf = (lastChildIndex / sizeof(Rect)) % rectanglesPerBlock;
      // If the next iteration will overflow the buffer, we need to save the first index's value and read the next block
      if (firstIndexInBuf > lastIndexInBuf) {
        bufferOverflow = true;
        previousRectx1 = rectBuffer[firstIndexInBuf].x1;
        previousRecty1 = rectBuffer[firstIndexInBuf].y1;
        break;
      }
      if (nodeNumber == nodesPerBlock - 1) {
        binNodePageWrite(firstTreeFile,nodeBuffer);
      }
      if (nodeNumber == amountOfNodes) break;
    }

  }
  if (nodeNumber != nodesPerBlock -1) {
    binNodePageWrite(firstTreeFile,nodeBuffer,nodeNumber);
  }
  delete[] rectBuffer;
  delete[] nodeBuffer;
  firstTreeFile.close();
  sortedRects.close();
  return this->buildTreeRecursive(1, nodeNumber, currentFilename);
}

unsigned int NearestXAlgorithm::buildTreeRecursive(unsigned int fileIndex, unsigned int currentNodeAmount, std::string previousTreeFileName) {
  // This file is the root
  if (currentNodeAmount == 1) {
    return fileIndex - 1;
  }
  std::ifstream previousTreeFile(previousTreeFileName,std::ios::in | std::ios::binary);
  previousTreeFile.seekg(0,std::ios::beg);
  RTreeNode* previousNodeBuffer = new RTreeNode[nodesPerBlock];
  RTreeNode* currentNodeBuffer = new RTreeNode[nodesPerBlock];
  unsigned int amountOfNodes = std::ceil((double)currentNodeAmount / this->maxNodeCapacity);
  std::string currentFileName = "NearestXRTree" + std::to_string(fileIndex) + ".bin";
  std::ofstream currentTreeFile(currentFileName,std::ios::out | std::ios::binary | std::ios::trunc);
  unsigned int nodeNumber = 0;
  unsigned int firstChildIndex, potentialLastChildIndex, blockLimit, lastChildIndex;
  firstChildIndex = potentialLastChildIndex = blockLimit = lastChildIndex = 0;
  unsigned int size = sizeof(RTreeNode);
  while (nodeNumber < amountOfNodes) {
    binNodePageRead(previousTreeFile,previousNodeBuffer);    
    potentialLastChildIndex = firstChildIndex + (this->maxNodeCapacity - 1) * sizeof(RTreeNode);
    int dataRead = previousTreeFile.gcount();
    int currentPos = previousTreeFile.tellg();
    if (currentPos == -1) {
      if (previousTreeFile.eof()) {
        blockLimit = lastChildIndex == 0 ? dataRead - (int) sizeof(RTreeNode): std::min(blockLimit,lastChildIndex);
      }
    } else {
      blockLimit = currentPos - (unsigned int) sizeof(RTreeNode);
    }
    lastChildIndex = std::min(potentialLastChildIndex, blockLimit);
    while (lastChildIndex <= blockLimit) {
      RTreeNode node;
      node.firstChildIndex = firstChildIndex;
      node.lastChildIndex = lastChildIndex;
      unsigned int firstIndexInBuf = (firstChildIndex / size) % nodesPerBlock;
      unsigned int lastIndexInBuf = (lastChildIndex / size) % nodesPerBlock;
      node.MBR.x1 = previousNodeBuffer[firstIndexInBuf].MBR.x1;
      node.MBR.y1 = previousNodeBuffer[firstIndexInBuf].MBR.y1;
      node.MBR.x2 = previousNodeBuffer[lastIndexInBuf].MBR.x2;
      node.MBR.y2 = previousNodeBuffer[lastIndexInBuf].MBR.y2;
      currentNodeBuffer[nodeNumber] = node;
      nodeNumber++;
      firstChildIndex = lastChildIndex + sizeof(RTreeNode);
      potentialLastChildIndex = firstChildIndex + (this->maxNodeCapacity - 1) * sizeof(RTreeNode);
      lastChildIndex = std::min(potentialLastChildIndex, blockLimit);
      if (nodeNumber == nodesPerBlock - 1) {
        binNodePageWrite(currentTreeFile,currentNodeBuffer);
        break;
      }
      if (nodeNumber == amountOfNodes) break;
    }
  }
  if (nodeNumber != nodesPerBlock -1) {
    binNodePageWrite(currentTreeFile,currentNodeBuffer,nodeNumber);
  }
  previousTreeFile.close();
  currentTreeFile.close();
  delete[] previousNodeBuffer;
  delete[] currentNodeBuffer;
  return this->buildTreeRecursive(fileIndex + 1, nodeNumber, currentFileName);
}