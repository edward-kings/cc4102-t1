#include "RTree.hpp"
#include <iostream>
#include <math.h>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include "../algoritmos/RTreeAlgorithm.hpp"
#include "../disk_manipulation_functions/binaryFileFunctions.hpp"

/**
 * @brief Constructor de la clase RTree.
 * \param maxNodeCapacity Cantidad máxima de hijos por nodo.
 * \param algorithm Algoritmo a utilizar para construir el RTree.
 * \param numberOfRects Cantidad de rectángulos que componen el RTree.
*/
RTree::RTree(int maxNodeCapacity, RTreeAlgorithm* algorithm, unsigned int numberOfRects) {
  this->maxNodeCapacity = maxNodeCapacity;
  this->algorithm = algorithm;
  this->numberOfRects = numberOfRects;
  this->totalSearchIOs = 0;
}

/**
 * @brief Método que retorna el contador de I/Os realizadas en la última búsqueda.
*/
unsigned int RTree::getTotalSearchIOs() {
  return this->totalSearchIOs;
}

/**
 * @brief Método que resetea el contador de I/Os realizadas en la última búsqueda.
*/
void RTree::resetTotalSearchIOs() {
  this->totalSearchIOs = 0;
}

/**
 * @brief Método que construye un RTree a partir de un archivo de rectángulos con un algoritmo.
 * \param filename Nombre del archivo de rectángulos.
*/
void RTree::buildTreeFromFile(std::string filename) {
  this->algorithm->buildTree(filename);
  this->treeFile = &this->algorithm->getTreeFile();
  this->leavesFile = &this->algorithm->getLeavesFile();
}

/**
 * @brief Método que verifica si 2 rectangulos intersecan.
 * \param queryRegion Rectángulo que representa la query.
 * \param MBR Rectángulo que representa el MBR de un nodo o el rectángulo de una hoja.
*/
bool RTree::intersects(Rect queryRegion, Rect MBR) {
  bool xOverlap = queryRegion.x2 >= MBR.x1 && queryRegion.x1 <= MBR.x2;
  bool yOverlap = queryRegion.y2 >= MBR.y1 && queryRegion.y1 <= MBR.y2;

  return xOverlap && yOverlap;
}

/**
 * @brief Método que realiza una búsqueda en el RTree.
 * \param region Rectángulo que representa la query.
*/
std::vector<Rect> RTree::search(Rect region) {
  std::vector<Rect> result;
  this->searchRecursive(region, 0, 0, result);
  return result;
}

/**
 * @brief Método que realiza una búsqueda en el RTree de forma recursiva.
 * \param region Rectángulo que representa la query.
 * \param firstChildIndex Índice del primer hijo del nodo, negativo o 0 si el hijo es hoja.
 * \param lastChildIndex Índice del último hijo del nodo, negativo o 0 si el hijo es hoja.
 * \param result Vector que contiene los rectángulos que intersectan con la query.
*/
void RTree::searchRecursive(Rect region, long long firstChildIndex, long long lastChildIndex, std::vector<Rect>& result) {
  // Si los hijos son hojas
  if (firstChildIndex < 0 || lastChildIndex < 0) {
    long long first = -firstChildIndex / sizeof(Rect);
    long long last = -lastChildIndex / sizeof(Rect);
    long long buffSize = last - first + 1;
    Rect* rectBuffer = new Rect[buffSize];
    std::istream& leavesFile = *this->leavesFile;
    leavesFile.seekg(firstChildIndex, std::ios::beg);
    leavesFile.read(reinterpret_cast<char*>(rectBuffer), sizeof(Rect) * buffSize);
    this->totalSearchIOs += (unsigned int)std::ceil((double)buffSize / rectanglesPerBlock);
    for (long long i = 0; i < buffSize; i++) {
      if (RTree::intersects(region, rectBuffer[i])) {
        result.push_back(rectBuffer[i]);
      }
    }
    delete[] rectBuffer;
  } else {
    long long first = firstChildIndex / sizeof(RTreeNode);
    long long last = lastChildIndex / sizeof(RTreeNode);
    long long buffSize = last - first + 1;
    RTreeNode* nodeBuffer = new RTreeNode[buffSize];
    std::istream& treeFile = *this->treeFile;
    treeFile.seekg(firstChildIndex, std::ios::beg);
    treeFile.read(reinterpret_cast<char*>(nodeBuffer), sizeof(RTreeNode) * buffSize);
    this->totalSearchIOs += (unsigned int)std::ceil((double)buffSize / nodesPerBlock);
    for (long long i = 0; i < buffSize; i++) {
      if (RTree::intersects(region, nodeBuffer[i].MBR)) {
        this->searchRecursive(region, nodeBuffer[i].firstChildIndex, nodeBuffer[i].lastChildIndex, result);
      }
    }
    delete[] nodeBuffer;
  }
}
