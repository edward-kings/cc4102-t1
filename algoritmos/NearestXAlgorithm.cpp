#include <iostream>
#include <fstream>
#include <istream>
#include <algorithm>
#include <string>
#include <math.h>
#include "NearestXAlgorithm.hpp"
#include "../estructuras/Rect.hpp"
#include "../disk_manipulation_functions/binaryFileFunctions.hpp"
#include "../estructuras/RTreeNode.hpp"

/**
 * @brief Constructor de la clase NearestXAlgorithm.
 * \param maxNodeCapacity Cantidad máxima de hijos por nodo.
 * \param numberOfRects Cantidad de rectángulos que componen el RTree.
*/
NearestXAlgorithm::NearestXAlgorithm(unsigned int maxNodeCapacity, unsigned int numberOfRects) {
  this->maxNodeCapacity = maxNodeCapacity;
  this->numberOfRects = numberOfRects;
}

/**
 * @brief Getter del archivo que contiene los nodos internos del RTree.
*/
std::istream& NearestXAlgorithm::getTreeFile() {
  return this->treeFile;
}

/**
 * @brief Getter del archivo que contiene las hojas del RTree.
*/
std::istream& NearestXAlgorithm::getLeavesFile() {
  return this->leavesFile;
}

/**
 * @brief Método que determina el criterio de ordenamiento de los rectángulos.
 * Nearest X ordena los rectángulos por su centro en el eje X.
*/
bool NearestXAlgorithm::orderCriteria(const Rect& rect1, const Rect& rect2) { 
  int rect1Center = floor(rect1.x1 + rect1.x2 / 2);
  int rect2Center = floor(rect2.x1 + rect2.x2 / 2);
  return rect1Center < rect2Center;
}

/**
 * @brief Método que construye un RTree utilizando el algoritmo NearestX.
 * \param filename Nombre del archivo de rectángulos.
*/
void NearestXAlgorithm::buildTree(std::string filename) {
  std::fstream rectFile(filename,std::ios::in | std::ios::binary);
  // Se lee el archivo de rectángulos y se ordenan por su centro en el eje X
  unsigned int bufferSize = this->numberOfRects;
  Rect* rectBuffer = new Rect[bufferSize];
  rectFile.read(reinterpret_cast<char*>(rectBuffer),sizeof(Rect) * bufferSize);
  rectFile.close();
  auto orderLambda = [this](const Rect& rect1, const Rect& rect2){ return this->orderCriteria(rect1,rect2); };
  std::sort(rectBuffer,rectBuffer + bufferSize,orderLambda);
  std::fstream& sortedRects = this->leavesFile;
  sortedRects = std::fstream("sortedRects.bin",std::ios::out | std::ios::in | std::ios::binary | std::ios::trunc);
  sortedRects.write(reinterpret_cast<char*>(rectBuffer),sizeof(Rect) * bufferSize);
  // amountOfNodes = n/M: cantidad de nodos internos
  unsigned int amountOfNodes = std::ceil((double)numberOfRects / this->maxNodeCapacity);
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
      currentRect = std::min(currentRect + this->maxNodeCapacity - 1, this->numberOfRects - 1);
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
    amountOfNodes = std::ceil((double)amountOfNodes / this->maxNodeCapacity);
    nodeBuffer.push_back(std::vector<RTreeNode>(amountOfNodes));
    for (unsigned int i = 0; i < amountOfNodes; i++) {
      RTreeNode node;
      node.MBR.x1 = nodeBuffer[currentLevel - 1][currentNodeIndex].MBR.x1;
      node.MBR.y1 = nodeBuffer[currentLevel - 1][currentNodeIndex].MBR.y1;
      node.firstChildIndex = (currentNodeIndex + 1) * sizeof(RTreeNode) * currentLevel;
      currentNodeIndex = std::min((unsigned long long)(currentNodeIndex + this->maxNodeCapacity - 1),(unsigned long long)(nodeBuffer[currentLevel -1].size() - 1));
      node.MBR.x2 = nodeBuffer[currentLevel - 1][currentNodeIndex].MBR.x2;
      node.MBR.y2 = nodeBuffer[currentLevel - 1][currentNodeIndex].MBR.y2;
      node.lastChildIndex = (currentNodeIndex + 1) * sizeof(RTreeNode) * currentLevel;
      nodeBuffer[currentLevel][i] = node;
    }
    currentLevel++;
    currentNodeIndex = 0;
  }
  delete[] rectBuffer;
  RTreeNode* nodeBufferArray = new RTreeNode[(int)(std::ceil((double)numberOfRects / this->maxNodeCapacity))];
  std::fstream& nearestXFile = this->treeFile;
  nearestXFile = std::fstream("NearestXRTree.bin",std::ios::out | std::ios::in |std::ios::binary | std::ios::trunc);
  for (int i = nodeBuffer.size() - 1; i >= 0; i--) {
    for (unsigned int j = 0; j < nodeBuffer[i].size(); j++) {
      nodeBufferArray[j] = nodeBuffer[i][j];
    }
    nearestXFile.write(reinterpret_cast<char*>(nodeBufferArray),sizeof(RTreeNode) * nodeBuffer[i].size());
  }
  delete[] nodeBufferArray;
}