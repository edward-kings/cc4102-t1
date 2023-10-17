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


unsigned int NearestXAlgorithm::getNumberOfRects() {
  return this->numberOfRects;
}

unsigned int NearestXAlgorithm::getMaxNodeCapacity() {
  return this->maxNodeCapacity;
}

/**
 * @brief Getter para el nombre del algoritmo
*/
std::string NearestXAlgorithm::getAlgorithmName() {
  return "NearestX";
}

/**
 * @brief Getter del archivo que contiene los nodos internos del RTree.
*/
std::istream* NearestXAlgorithm::getTreeFile() {
  return this->treeFile;
}

/**
 * @brief Getter del archivo que contiene las hojas del RTree.
*/
std::istream* NearestXAlgorithm::getLeavesFile() {
  return this->leavesFile;
}

void NearestXAlgorithm::setTreeFile(std::fstream* fileStream) {
  this->treeFile = fileStream;
}

void NearestXAlgorithm::setLeavesFile(std::fstream* fileStream) {
  this->leavesFile = fileStream;
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


void NearestXAlgorithm::preProcess(Rect* rects, unsigned int arraySize) {
  auto orderLambda = [this](const Rect& rect1, const Rect& rect2) { return this->orderCriteria(rect1, rect2); };
  std::stable_sort(rects, rects + arraySize, orderLambda);
}