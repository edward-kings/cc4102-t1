#include "RTreeAlgorithm.hpp"
#include <algorithm>
#include <math.h>
#include <iostream>

/**
 * @brief Método que construye un RTree utilizando el algoritmo.
 * \param filename Nombre del archivo de rectángulos.
*/
void RTreeAlgorithm::buildTree(std::string filename) {
  std::fstream rectFile(filename, std::ios::in | std::ios::binary);
  // Se lee el archivo de rectángulos y se ordenan por su centro en el eje X
  unsigned int bufferSize = this->getNumberOfRects();
  Rect* rectBuffer = new Rect[bufferSize];
  rectFile.seekg(0, std::ios::beg);
  rectFile.read(reinterpret_cast<char*>(rectBuffer), sizeof(Rect) * bufferSize);
  rectFile.close();
  this->preProcess(rectBuffer, bufferSize);
  std::fstream* sortedRects = new std::fstream;
  *sortedRects = std::fstream("sortedRects" + this->getAlgorithmName() +".bin", std::ios::out | std::ios::in | std::ios::binary | std::ios::trunc);
  sortedRects->write(reinterpret_cast<char*>(rectBuffer), sizeof(Rect) * bufferSize);
  this->setLeavesFile(sortedRects);
  // amountOfNodes = n/M: cantidad de nodos internos
  unsigned int numberOfRects = this->getNumberOfRects();
  unsigned int maxNodeCapacity = this->getMaxNodeCapacity();
  unsigned int amountOfNodes = std::ceil((double)numberOfRects / maxNodeCapacity);
  unsigned int nodeNumber = 0;
  unsigned int currentRect = 0;
  // Indica la altura actual del árbol
  unsigned int currentLevel = 0;
  std::vector<std::vector<RTreeNode>> nodeBuffer;
  nodeBuffer.push_back(std::vector<RTreeNode>(amountOfNodes));
  while (nodeNumber < amountOfNodes) {
    RTreeNode node;
    node.MBR.x1 = rectBuffer[currentRect].x1;
    node.MBR.y1 = rectBuffer[currentRect].y1;
    // Las hojas tienen índices negativos para diferenciarlos de los nodos internos
    node.firstChildIndex = -(currentRect * sizeof(Rect));
    currentRect = std::min(currentRect + maxNodeCapacity - 1, numberOfRects - 1);
    node.MBR.x2 = rectBuffer[currentRect].x2;
    node.MBR.y2 = rectBuffer[currentRect].y2;
    node.lastChildIndex = -(currentRect * sizeof(Rect));
    nodeBuffer[currentLevel][nodeNumber] = node;
    currentRect++;
    nodeNumber++;
  }
  currentLevel++;
  unsigned int currentNodeIndex = 0;
  // Mientras queden nodos por crear
  while (amountOfNodes > 1) {
    amountOfNodes = std::ceil((double)amountOfNodes / maxNodeCapacity);
    nodeBuffer.push_back(std::vector<RTreeNode>(amountOfNodes));
    for (unsigned int i = 0; i < amountOfNodes; i++) {
      RTreeNode node;
      node.MBR.x1 = nodeBuffer[currentLevel - 1][currentNodeIndex].MBR.x1;
      node.MBR.y1 = nodeBuffer[currentLevel - 1][currentNodeIndex].MBR.y1;
      node.firstChildIndex = (currentNodeIndex + 1) * sizeof(RTreeNode) * currentLevel;
      currentNodeIndex = std::min((unsigned long long)(currentNodeIndex + maxNodeCapacity - 1), (unsigned long long)(nodeBuffer[currentLevel - 1].size() - 1));
      node.MBR.x2 = nodeBuffer[currentLevel - 1][currentNodeIndex].MBR.x2;
      node.MBR.y2 = nodeBuffer[currentLevel - 1][currentNodeIndex].MBR.y2;
      node.lastChildIndex = (currentNodeIndex + 1) * sizeof(RTreeNode) * currentLevel;
      nodeBuffer[currentLevel][i] = node;
      currentNodeIndex ++;
    }
    currentLevel++;
    currentNodeIndex = 0;
  }
  RTreeNode* nodeBufferArray = new RTreeNode[(int)(std::ceil((double)numberOfRects / maxNodeCapacity))];
  std::fstream* algorithmTreeFile = new std::fstream;
  this->setTreeFile(algorithmTreeFile);
  *algorithmTreeFile = std::fstream(this->getAlgorithmName() + "RTree.bin", std::ios::out | std::ios::in | std::ios::binary | std::ios::trunc);
  for (int i = nodeBuffer.size() - 1; i >= 0; i--) {
    for (unsigned int j = 0; j < nodeBuffer[i].size(); j++) {
      nodeBufferArray[j] = nodeBuffer[i][j];
    }
    algorithmTreeFile->write(reinterpret_cast<char*>(nodeBufferArray), sizeof(RTreeNode) * nodeBuffer[i].size());
  }
  delete[] nodeBufferArray;
  delete[] rectBuffer;
  this->ensureDifferentChildren();
}

/**
 * @brief Método que se encarga de asegurar que todos los nodos tengan hijos diferentes.
*/
void RTreeAlgorithm::ensureDifferentChildren() {
  std::fstream treeFile(this->getAlgorithmName() + "RTree.bin", std::ios::out | std::ios::in | std::ios::binary);
  treeFile.seekg(0, std::ios::end);
  unsigned int fileSize = treeFile.tellg();
  treeFile.seekg(0, std::ios::beg);
  unsigned int nodeSize = sizeof(RTreeNode);
  unsigned int maxNodeCapacity = this->getMaxNodeCapacity();
  unsigned int nodesPerBlock = diskBlockSize / nodeSize;
  unsigned int blocks = std::ceil((double)fileSize / diskBlockSize);
  unsigned int currentNodeIndex = 0;
  for (unsigned int i = 0; i < blocks; i++) {
    unsigned int nodesInBlock = (i == blocks - 1) ? std::ceil((double)(fileSize % diskBlockSize) / nodeSize) : nodesPerBlock;
    RTreeNode* nodeBuffer = new RTreeNode[nodesInBlock];
    treeFile.read(reinterpret_cast<char*>(nodeBuffer), nodeSize * nodesInBlock);
    for (unsigned int j = 0; j < nodesInBlock; j++) {
      RTreeNode& node = nodeBuffer[j];
      if (node.firstChildIndex < 0) {
        continue;
      }
      unsigned int firstChildIndex = std::abs(node.firstChildIndex) / nodeSize;
      unsigned int lastChildIndex = std::abs(node.lastChildIndex) / nodeSize;
      for (unsigned int k = firstChildIndex; k <= lastChildIndex; k++) {
        if (k >= nodesInBlock) {
         break; // stop looping if k is out of bounds
        }
        RTreeNode& childNode = nodeBuffer[k];
        if (childNode.firstChildIndex < 0) {
          continue;
        }
        if (k == j) {
          childNode.firstChildIndex = -1;
          childNode.lastChildIndex = -1;
        } else {
          childNode.firstChildIndex = (k + 1) * nodeSize;
          childNode.lastChildIndex = (k + 1) * nodeSize;
        }
      }
    }
    treeFile.seekp(i * diskBlockSize, std::ios::beg);
    treeFile.write(reinterpret_cast<char*>(nodeBuffer), nodeSize * nodesInBlock);
    delete[] nodeBuffer;
  }
}
