#include <iostream>
#include <fstream>
#include <istream>
#include <algorithm>
#include <string>
#include <math.h>
#include "SortTileRecursiveAlgorithm.hpp"
#include "../estructuras/Rect.hpp"
#include "../disk_manipulation_functions/binaryFileFunctions.hpp"
#include "../estructuras/RTreeNode.hpp"

/**
 * @brief Constructor de la clase SortTileRecursiveAlgorithm.
 * \param maxNodeCapacity Cantidad máxima de hijos por nodo.
 * \param numberOfRects Cantidad de rectángulos que componen el RTree.
*/
SortTileRecursiveAlgorithm::SortTileRecursiveAlgorithm(unsigned int maxNodeCapacity, unsigned int numberOfRects) {
  this->maxNodeCapacity = maxNodeCapacity;
  this->numberOfRects = numberOfRects;
}


unsigned int SortTileRecursiveAlgorithm::getNumberOfRects() {
  return this->numberOfRects;
}

unsigned int SortTileRecursiveAlgorithm::getMaxNodeCapacity() {
  return this->maxNodeCapacity;
}

/**
 * @brief Getter para el nombre del algoritmo
*/
std::string SortTileRecursiveAlgorithm::getAlgorithmName() {
  return "NearestX";
}

/**
 * @brief Getter del archivo que contiene los nodos internos del RTree.
*/
std::istream* SortTileRecursiveAlgorithm::getTreeFile() {
  return this->treeFile;
}

/**
 * @brief Getter del archivo que contiene las hojas del RTree.
*/
std::istream* SortTileRecursiveAlgorithm::getLeavesFile() {
  return this->leavesFile;
}

void SortTileRecursiveAlgorithm::setTreeFile(std::fstream* fileStream) {
  this->treeFile = fileStream;
}

void SortTileRecursiveAlgorithm::setLeavesFile(std::fstream* fileStream) {
  this->leavesFile = fileStream;
}

/**
 * @brief Método que determina el criterio de ordenamiento de los rectángulos.
 * SortTileRecursive ordena los rectángulos por su centro en el eje X y después en el Y.
*/
bool SortTileRecursiveAlgorithm::orderCriteria(const Rect& rect1, const Rect& rect2) {
  int rect1Center = floor(rect1.x1 + rect1.x2 / 2);
  int rect2Center = floor(rect2.x1 + rect2.x2 / 2);
  return rect1Center < rect2Center;
}

bool SortTileRecursiveAlgorithm::orderCriteriaY(const Rect& rect1, const Rect& rect2) {
  int rect1Center = floor(rect1.y1 + rect1.y2 / 2);
  int rect2Center = floor(rect2.y1 + rect2.y2 / 2);
  return rect1Center < rect2Center;
}

void SortTileRecursiveAlgorithm::preProcess(Rect* rects, unsigned int arraySize) {
  auto orderLambda = [this](const Rect& rect1, const Rect& rect2) { return this->orderCriteria(rect1, rect2); };
  auto orderLambda2 = [this](const Rect& rect1, const Rect& rect2) { return this->orderCriteriaY(rect1, rect2); };
  std::stable_sort(rects, rects + arraySize, orderLambda);
  unsigned int subSetAmount = std::ceil(std::sqrt((double)numberOfRects / maxNodeCapacity));
  unsigned int subSetSize = subSetAmount * this->maxNodeCapacity;
  unsigned int firstSubSetEnd = subSetSize;
  std::stable_sort(rects, rects + firstSubSetEnd, orderLambda2);
  for (int i = 1; i < subSetAmount; i++) {
    unsigned int subsetStart = subSetSize * i + 1;
    unsigned int subsetEnd = std::min(subSetSize * (i+1), numberOfRects);
    std::stable_sort(rects + subsetStart, rects + subsetEnd, orderLambda2);
  }
}
