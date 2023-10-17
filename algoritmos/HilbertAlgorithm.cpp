#include "HilbertAlgorithm.hpp"
#include <math.h>
#include <iostream>
#include <algorithm>

HilbertAlgorithm::HilbertAlgorithm(unsigned int maxNodeCapacity, unsigned int numberOfRects) {
  this->maxNodeCapacity = maxNodeCapacity;
  this->numberOfRects = numberOfRects;
  this->hilbertSquareN = 1 << 19;
}

unsigned int HilbertAlgorithm::getNumberOfRects() {
  return this->numberOfRects;
}

unsigned int HilbertAlgorithm::getMaxNodeCapacity() {
  return this->maxNodeCapacity;
}


/**
 * @brief Getter para el nombre del algoritmo
*/
std::string HilbertAlgorithm::getAlgorithmName() {
  return "Hilbert";
}

/**
 * @brief Getter del archivo que contiene los nodos internos del RTree.
*/
std::istream* HilbertAlgorithm::getTreeFile() {
  return this->treeFile;
}

/**
 * @brief Getter del archivo que contiene las hojas del RTree.
*/
std::istream* HilbertAlgorithm::getLeavesFile() {
  return this->leavesFile;
}

void HilbertAlgorithm::setTreeFile(std::fstream* fileStream) {
  this->treeFile = fileStream;
}

void HilbertAlgorithm::setLeavesFile(std::fstream* fileStream) {
  this->leavesFile = fileStream;
}

unsigned long long HilbertAlgorithm::coordsToHilbertD (int n, int x, int y) {
  int rx, ry;
  unsigned long long d = 0;
  for (unsigned long long s = n/2; s > 0; s /= 2) {
    rx = (x & s) > 0 ? 1 : 0;
    ry = (y & s) > 0 ? 1 : 0;
    d += s * s * ((3 * rx) ^ ry);
    rotate(s, &x, &y, rx, ry);
  }
  return d;
}

void HilbertAlgorithm::rotate(int n, int* x, int* y, int rx, int ry) {
  int t;
  if (ry == 0) {
    if (rx == 1) {
      *x = n-1 - *x;
      *y = n-1 - *y;
    }
    t  = *x;
    *x = *y;
    *y = t;
  }
}

/**
 * @brief Método que determina el criterio de ordenamiento de los rectángulos.
 * Hilbert ordena los rectángulos según el valor "d" de sus centros en la curva de hilbert.
*/
bool HilbertAlgorithm::orderCriteria(const Rect& rect1, const Rect& rect2) {
  unsigned int rect1CenterX = floor(rect1.x1 + rect1.x2 / 2);
  unsigned int rect2CenterX = floor(rect2.x1 + rect2.x2 / 2);
  unsigned int rect1CenterY = floor(rect1.y1 + rect2.y2 / 2);
  unsigned int rect2CenterY = floor(rect2.y1 + rect2.y2 / 2);

  unsigned long long rect1D = this->coordsToHilbertD(this->hilbertSquareN, rect1CenterX, rect1CenterY);
  unsigned long long rect2D = this->coordsToHilbertD(this->hilbertSquareN, rect2CenterX, rect2CenterY);

  return rect1D < rect2D;
}

void HilbertAlgorithm::preProcess(Rect* rects, unsigned int arraySize) {
  auto orderLambda = [this](const Rect& rect1, const Rect& rect2) { return this->orderCriteria(rect1, rect2); };
  std::stable_sort(rects, rects + arraySize, orderLambda);
}