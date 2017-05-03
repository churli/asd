#ifndef TOMITA_H_
#define TOMITA_H_

#include <stdlib.h>
#include "Common.h"
#include "Logger.h"
#include "AdjacencyMatrix.h"
#include "Sets.h"
#include "BronKerbosch.h"

void Tomita_expand(Set * R, Set * Q, Set * Qmax, long * No);

void Tomita_numberSort(Set * R, long * No);

void startTomita();

#endif // TOMITA_H_
