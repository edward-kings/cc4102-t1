#ifndef SORT_TILE_RECURSIVE_ALGORITHM
#define SORT_TILE_RECURSIVE_ALGORITHM

#include "RTreeAlgorithm.hpp"
#include <string>
#include "../estructuras/Rect.hpp"
#include <fstream>
/**
 * @brief Clase que construye un RTree utilizando el algoritmo NearestX.
 * \param treeFile Archivo que contiene los nodos internos del RTree.
 * \param leavesFile Archivo que contiene las hojas del RTree.
 * \param maxNodeCapacity Cantidad máxima de hijos por nodo.
 * \param numberOfRects Cantidad de rectángulos que componen el RTree.
*/
class SortTileRecursiveAlgorithm : public RTreeAlgorithm {
public:
  /**
   * @brief Destructor de la clase SortTileRecursiveAlgorithm.
   * Cierra los archivos treeFile y leavesFile.
  */
  ~SortTileRecursiveAlgorithm() {
    this->treeFile->close();
    this->leavesFile->close();
    delete this->treeFile;
    delete this->leavesFile;
  }
  SortTileRecursiveAlgorithm(unsigned int maxNodeCapacity, unsigned int numberOfRects);
  std::string getAlgorithmName() override;
  std::istream* getTreeFile() override;
  std::istream* getLeavesFile() override;
  unsigned int getNumberOfRects() override;
  unsigned int getMaxNodeCapacity() override;
private:
  bool orderCriteria(const Rect& rect1, const Rect& rect2) override;
  bool orderCriteriaY(const Rect& rect1, const Rect& rect2);
  std::fstream* treeFile;
  std::fstream* leavesFile;
  void setTreeFile(std::fstream* fileStream) override;
  void setLeavesFile(std::fstream* fileStream) override;
  unsigned int maxNodeCapacity;
  unsigned int numberOfRects;
  void preProcess(Rect* rects, unsigned int preProcess) override;
};

#endif // SORT_TILE_RECURSIVE_ALGORITHM