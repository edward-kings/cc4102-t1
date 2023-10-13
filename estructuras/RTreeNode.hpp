#ifndef RTREE_NODE
#define RTREE_NODE

#include "Rect.hpp"

/**
 * @brief Estructura que representa un nodo interno del RTree.
*/
struct RTreeNode {
  Rect MBR;
  long long firstChildIndex;
  long long lastChildIndex;
};

#endif
