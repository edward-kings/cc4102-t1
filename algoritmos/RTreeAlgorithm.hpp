#ifndef RTREE_ALGORITHM_H
#define RTREE_ALGORITHM_H

#include "../estructuras/Rect.hpp"
#include "../estructuras/RTreeNode.hpp"
#include "../disk_manipulation_functions/binaryFileFunctions.hpp"
#include <vector>
#include <cstdio>

extern unsigned int nodesPerBlock;

class RTreeAlgorithm {
public:
  virtual ~RTreeAlgorithm() {}
  virtual void buildTree(FILE*& rectsFile) = 0;
  virtual std::vector<FILE*> getTreeFiles() = 0;
  virtual void setTreeFiles(std::vector<FILE*> &treeFiles) = 0;
protected:
  virtual int getNumberOfRects() = 0;
  virtual int getMaxNodeCapacity() = 0;
  virtual bool orderCriteria(const Rect& rect1, const Rect& rect2) = 0;
  virtual FILE*& getSortedRectsFile() = 0;
  void externalMergeSort(FILE*& rectsFile);
};

#endif // RTREE_ALGORITHM_H