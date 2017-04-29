#ifndef SETS_H_
#define SETS_H_

#include <stdlib.h>
#include "Common.h"
#include "Logger.h"

typedef struct SetElement
{
  long serial;
  long degree;
  // List interface
  struct SetElement * next;
  // Tree interface
  // int h; // Height for AVL
  // struct SetElement * left;
  // struct SetElement * right;
  // char deleted; // 0 if valid, 1 if deleted
} SetElement;

typedef struct Set
{
  long size; // Number of active nodes
  // List interface
  SetElement * first;
  SetElement * last;
  // Tree interface
  // SetElement * root;
  // long deleted; // Number of deleted (inctive) nodes
} Set;

SetElement * SetElement_new(long serial, long degree);

Set * Set_new();

bool Set_isEmpty(Set * set);

// long AVLTreeSet_compare(SetElement * a, SetElement * b);

// int AVLTreeSet_getHeight(SetElement * node);

// SetElement * AVLTreeSet_leftRotation(SetElement * node);

// SetElement * AVLTreeSet_rightRotation(SetElement * node);

// SetElement * AVLTreeSet_add(SetElement * node, SetElement * newElement);

// void Set_addAVL(Set * set, SetElement * newElement);

void Set_add(Set * set, SetElement * newElement);

// SetElement * AVLTreeSet_search(SetElement * node, SetElement * target);

// void Set_clearDeletedAndRebuild(Set * set);

// void Set_removeAVL(Set * set, SetElement * target);

void Set_remove(Set * set, long serial);

void Set_clear(Set * set);

void Set_print(Set * set, char * setName);

void Set_append(Set * set, SetElement * newElement);

Set * Set_union(Set * a, Set * b);

Set * Set_intersection(Set * a, Set * b);

Set * Set_difference(Set * a, Set * b);

SetElement * Set_getFirstInUnion(Set * a, Set * b);

#endif // SETS_H_
