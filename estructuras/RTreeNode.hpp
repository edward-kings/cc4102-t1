#ifndef RTREE_NODE
#define RTREE_NODE

#include "Rect.hpp"

struct RTreeNode {
  Rect MBR;
  unsigned long firstChildIndex;
  unsigned long lastChildIndex;
};

#endif
