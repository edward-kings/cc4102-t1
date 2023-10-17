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
  void buildTree(std::string filename);
  virtual unsigned int getNumberOfRects() = 0;
  virtual unsigned int getMaxNodeCapacity() = 0;
  virtual std::istream* getTreeFile() = 0;
  virtual std::istream* getLeavesFile() = 0;
  virtual std::string getAlgorithmName() = 0;
private:
  virtual bool orderCriteria(const Rect& rect1, const Rect& rect2) = 0;
  virtual void setTreeFile(std::fstream* fileStream) = 0;
  virtual void setLeavesFile(std::fstream* fileStream) = 0;
  virtual void preProcess(Rect* rectangles, unsigned int arraySize) = 0;
};

#endif // RTREE_ALGORITHM_H