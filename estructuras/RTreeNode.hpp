#ifndef RTREE_NODE
#define RTREE_NODE

#include "Rect.hpp"

struct RTreeNode {
  Rect MBR;
  unsigned int firstChildIndex;
  unsigned int lastChildIndex;
};

#endif
