#ifndef RTREE
#define RTREE

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "Rect.hpp"
#include "RTreeNode.hpp"
#include "../algoritmos/RTreeAlgorithm.hpp"
#include "../disk_manipulation_functions/binaryFileFunctions.hpp"

/**
 * @brief Clase que representa un RTree.
 * \param maxNodeCapacity Cantidad máxima de hijos por nodo.
 * \param algorithm Algoritmo a utilizar para construir el RTree.
 * \param numberOfRects Cantidad de rectángulos que componen el RTree.
 * \param totalSearchIOs Cantidad de I/Os realizadas en la última búsqueda.
 * \param treeFile Puntero al archivo que contiene los nodos internos del RTree.
 * \param leavesFile Puntero al archivo que contiene las hojas del RTree.
*/
class RTree {
public:
  RTree(int maxNodeCapacity, RTreeAlgorithm* algorithm, unsigned int numberOfRects);
  std::vector<Rect> search(Rect region);
  void buildTreeFromFile(std::string filename);
  std::string getAlgorithmName();
  unsigned int getTotalSearchIOs();
  void resetTotalSearchIOs();
private:
  unsigned int maxNodeCapacity;
  unsigned int numberOfRects;
  unsigned int totalSearchIOs;
  std::istream* treeFile;
  std::istream* leavesFile;
  RTreeAlgorithm* algorithm;
  static bool intersects(Rect region, Rect MBR);
  void searchRecursive(Rect region, long long firstChildIndex, long long lastChildIndex, std::vector<Rect>& result);
};


#endif