#ifndef NEAREST_X_ALGORITHM_H
#define NEAREST_X_ALGORITHM_H

#include "RTreeAlgorithm.hpp"
#include <string>
#include "../estructuras/Rect.hpp"
class NearestXAlgorithm : public RTreeAlgorithm {
public:
  NearestXAlgorithm(int maxNodeCapacity, unsigned int numberOfRects);
  unsigned int buildTree(std::string filename) override;
  std::string getTreeFileBaseName() override;
private:
  std::string treeFileBaseName;
  std::string sortedRectsFileName;
  int maxNodeCapacity;
  unsigned int numberOfRects;
  unsigned int buildTreeRecursive(unsigned int fileIndex, unsigned int currentNodeAmount, std::string previousTreeFileName);
protected:
  int getNumberOfRects() override;
  int getMaxNodeCapacity() override;
  bool orderCriteria(const Rect& rect1, const Rect& rect2) override;
  std::string getSortedRectsFileName() override;
  void setSortedRectsFileName(std::string sortedRectsFileName) override;
};

#endif // NEAREST_X_ALGORITHM_H