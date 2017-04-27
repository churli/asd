#ifndef BRON_KERBOSCH_H_
#define BRON_KERBOSCH_H_

#include <stdlib.h>
#include "Common.h"
#include "Logger.h"
#include "AdjacencyMatrix.h"

static long _maxCliqueSize;

typedef struct SetElement
{
  long serial;
  long degree;
  struct SetElement * next;
} SetElement;

typedef struct Set
{
  long size;
  SetElement * first;
  SetElement * last;
} Set;

SetElement * SetElement_new(long serial, long degree);

Set * Set_new();

bool Set_isEmpty(Set * set);

void Set_add(Set * set, SetElement * newElement);

void Set_append(Set * set, SetElement * newElement);

void Set_remove(Set * set, long serial);

void Set_clear(Set * set);

void Set_print(Set * set, char * setName);

Set * Set_union(Set * a, Set * b);

Set * Set_intersection(Set * a, Set * b);

Set * Set_difference(Set * a, Set * b);

SetElement * Set_getFirstInUnion(Set * a, Set * b);

Set * getNeighbours(long serial);

void onNewClique(long newCliqueSize);

void bronKerbosch(Set * R, Set * P, Set * X);

void startBronKerbosch();

#endif // BRON_KERBOSCH_H_
