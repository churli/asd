#ifndef VBRON_KERBOSCH_H_
#define VBRON_KERBOSCH_H_

#include <stdlib.h>
#include "Common.h"
#include "Logger.h"
#include "AdjacencyMatrix.h"
#include "VectorSet.h"

static long _maxCliqueSize;

void onVNewClique(long newCliqueSize);

void vBronKerbosch(char * R, char * P, char * X);

void startVBronKerbosch();

#endif // VBRON_KERBOSCH_H_
