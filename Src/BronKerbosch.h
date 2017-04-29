#ifndef BRON_KERBOSCH_H_
#define BRON_KERBOSCH_H_

#include <stdlib.h>
#include "Common.h"
#include "Logger.h"
#include "AdjacencyMatrix.h"
#include "Sets.h"

static long _maxCliqueSize;
static Set ** _neighboursSetArray;

void allocateNeighboursSetArray();

Set * computeNeighbours(long serial);

Set * getNeighbours(long serial);

void onNewClique(long newCliqueSize);

void bronKerbosch(Set * R, Set * P, Set * X);

void startBronKerbosch();

#endif // BRON_KERBOSCH_H_
