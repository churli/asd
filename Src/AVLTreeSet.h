#ifndef AVLTREE_SETS_H_
#define AVLTREE_SETS_H_

#include <stdlib.h>
#include "Common.h"
#include "Logger.h"

typedef struct TreeSetElement
{
  long serial;
  long degree;
  // List interface
  struct SetElement * prev;
  struct SetElement * next;
  // Tree interface
  // int h; // Height for AVL
  // struct SetElement * left;
  // struct SetElement * right;
  // char deleted; // 0 if valid, 1 if deleted
} SetElement;

typedef struct TreeSet
{
  long size; // Number of active nodes
  // List interface
  SetElement * first;
  SetElement * last;
  // Tree interface
  // SetElement * root;
  // long deleted; // Number of deleted (inctive) nodes
} Set;

// long AVLTreeSet_compare(SetElement * a, SetElement * b);

// int AVLTreeSet_getHeight(SetElement * node);

// SetElement * AVLTreeSet_leftRotation(SetElement * node);

// SetElement * AVLTreeSet_rightRotation(SetElement * node);

// SetElement * AVLTreeSet_add(SetElement * node, SetElement * newElement);

// void Set_addAVL(Set * set, SetElement * newElement);

// SetElement * AVLTreeSet_search(SetElement * node, SetElement * target);

// void Set_clearDeletedAndRebuild(Set * set);

// void Set_removeAVL(Set * set, SetElement * target);

#endif // AVLTREE_SETS_H_
