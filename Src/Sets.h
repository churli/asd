#ifndef SETS_H_
#define SETS_H_

#include <stdlib.h>
#include "Common.h"
#include "Logger.h"
#include "AdjacencyMatrix.h"

typedef struct SetElement
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

long Set_compare(SetElement * a, SetElement * b);

void Set_add(Set * set, SetElement * newElement);

// void Set_addByAscendingSerial(Set * set, SetElement * newElement);

long ColorSet_compare(long * No, SetElement * a, SetElement * b);

void ColorSet_add(long * No, Set * set, SetElement * newElement);

void Set_remove(Set * set, long serial);

void Set_removeLast(Set * set);

void Set_clear(Set * set);

void Set_free(Set * set);

void Set_print(Set * set, char * setName);

Set * Set_copy(Set * set);

void Set_append(Set * set, SetElement * newElement);

Set * Set_union(Set * a, Set * b);

Set * Set_intersection(Set * a, Set * b);

Set * ColorSet_intersection(long * No, Set * a, Set * b);

Set * Set_difference(Set * a, Set * b);

SetElement * Set_getFirstInUnion(Set * a, Set * b);

SetElement * Set_getFirstInIntersection(Set * a, Set * b);

SetElement * ColorSet_getFirstInIntersection(long * No, Set * a, Set * b);

Set * ColorSet_intersectWithNeighboursOf(long * No, Set * a, long serial);

SetElement * ColorSet_getFirstInIntersectionWithNeighboursOf(long * No, Set * a, long serial);

// Set * Set_reSortBySerial(SetElement * first);

#endif // SETS_H_
