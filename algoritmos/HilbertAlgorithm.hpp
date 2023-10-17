#ifndef HILBERT_ALGORITHM
#define HILBERT_ALGORITHM

#include "RTreeAlgorithm.hpp"

class HilbertAlgorithm : public RTreeAlgorithm {
public:
  /**
   * @brief Destructor de la clase NearestXAlgorithm.
   * Cierra los archivos treeFile y leavesFile.
  */
  ~HilbertAlgorithm() {
    this->treeFile->close();
    this->leavesFile->close();
    delete this->treeFile;
    delete this->leavesFile;
  }
  HilbertAlgorithm(unsigned int maxNodeCapacity, unsigned int numberOfRects);
  std::string getAlgorithmName() override;
  std::istream* getTreeFile() override;
  std::istream* getLeavesFile() override;
  unsigned int getNumberOfRects() override;
  unsigned int getMaxNodeCapacity() override;
private:
  bool orderCriteria(const Rect& rect1, const Rect& rect2) override;
  unsigned long long coordsToHilbertD(int n, int x, int y);
  void rotate(int n, int* x, int* y, int rx, int ry);
  std::fstream* treeFile;
  std::fstream* leavesFile;
  void setTreeFile(std::fstream* fileStream) override;
  void setLeavesFile(std::fstream* fileStream) override;
  unsigned int maxNodeCapacity;
  unsigned int numberOfRects;
  int hilbertSquareN;
  void preProcess(Rect* rects, unsigned int arraySize) override;
};

#endif //HILBERT_ALGORITHM.H
