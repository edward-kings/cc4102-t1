#ifndef NEAREST_X_ALGORITHM_H
#define NEAREST_X_ALGORITHM_H

#include "RTreeAlgorithm.hpp"

class NearestXAlgorithm : public RTreeAlgorithm {
public:
  NearestXAlgorithm(int maxNodeCapacity, unsigned long numberOfRects);
  void buildTree(FILE*& rectsFile) override;
  std::vector<FILE*> getTreeFiles() override;
  void setTreeFiles(std::vector<FILE*> &treeFiles) override;
private:
  std::vector<FILE*> treeFiles;;
  FILE* sortedRectsFile;
  int maxNodeCapacity;
  unsigned long numberOfRects;
  void buildTreeRecursive(unsigned long fileIndex, unsigned long currentNodeAmount);
protected:
  bool orderCriteria(const Rect& rect1, const Rect& rect2) override;
  FILE*& getSortedRectsFile() override;
};

#endif // NEAREST_X_ALGORITHM_H