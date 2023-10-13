#ifndef RTREE_ALGORITHM_H
#define RTREE_ALGORITHM_H

#include "../estructuras/Rect.hpp"
#include "../estructuras/RTreeNode.hpp"
#include "../disk_manipulation_functions/binaryFileFunctions.hpp"
#include <vector>
#include <cstdio>
#include <fstream>
#include <istream>

/**
 * @brief Interfaz que representa un algoritmo para construir un RTree.
*/
class RTreeAlgorithm {
public:
  virtual ~RTreeAlgorithm() {}
  virtual void buildTree(std::string filename) = 0;
  virtual std::istream& getTreeFile() = 0;
  virtual std::istream& getLeavesFile() = 0;
};

#endif // RTREE_ALGORITHM_H