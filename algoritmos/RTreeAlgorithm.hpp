#ifndef RTREE_ALGORITHM_H
#define RTREE_ALGORITHM_H

#include "../estructuras/Rect.hpp"
#include "../estructuras/RTreeNode.hpp"
#include "../disk_manipulation_functions/binaryFileFunctions.hpp"
#include <vector>
#include <cstdio>

class RTreeAlgorithm {
public:
  virtual ~RTreeAlgorithm() {}
  virtual unsigned int buildTree(std::string filename) = 0;
  virtual std::string getTreeFileBaseName() = 0;
protected:
  virtual int getNumberOfRects() = 0;
  virtual int getMaxNodeCapacity() = 0;
  virtual bool orderCriteria(const Rect& rect1, const Rect& rect2) = 0;
  virtual std::string getSortedRectsFileName() = 0;
  virtual void setSortedRectsFileName(std::string sortedRectsFileName) = 0;
  void externalMergeSort(std::string filename);
};

#endif // RTREE_ALGORITHM_H