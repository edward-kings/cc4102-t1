#ifndef NEAREST_X_ALGORITHM_H
#define NEAREST_X_ALGORITHM_H

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
class NearestXAlgorithm : public RTreeAlgorithm {
public:
  /**
   * @brief Destructor de la clase NearestXAlgorithm.
   * Cierra los archivos treeFile y leavesFile.
  */
  ~NearestXAlgorithm() {
    this->treeFile->close();
    this->leavesFile->close();
    delete this->treeFile;
    delete this->leavesFile;
  }
  NearestXAlgorithm(unsigned int maxNodeCapacity, unsigned int numberOfRects);
  std::string getAlgorithmName() override;
  std::istream* getTreeFile() override;
  std::istream* getLeavesFile() override;
  unsigned int getNumberOfRects() override;
  unsigned int getMaxNodeCapacity() override;
private:
  bool orderCriteria(const Rect& rect1, const Rect& rect2) override;
  std::fstream* treeFile;
  std::fstream* leavesFile;
  void setTreeFile(std::fstream* fileStream) override;
  void setLeavesFile(std::fstream* fileStream) override;
  unsigned int maxNodeCapacity;
  unsigned int numberOfRects;
  void preProcess(Rect* rects, unsigned int arraySize) override;
};

#endif // NEAREST_X_ALGORITHM_H