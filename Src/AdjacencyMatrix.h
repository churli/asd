#ifndef ADJACENCY_MATRIX_H_
#define ADJACENCY_MATRIX_H_

#include <stdlib.h>
#include "Common.h"
#include "Logger.h"

static char * _adjacencyVector;
static long _numGraphElements;

// Mem allocation (initialization)
void AM_initialize(long numGraphElements);

// Indexing
long AM_getIndex(long i, long j);
long AM_getCharIndex(long index);
short AM_getSubBitIndex(long index);

// Getting bits
bool AM_getBit(char byte, short i);

// Setting bits
void AM_setBit(char* byte, short i);

// High level operations, set * get
bool AM_getIfAdjacent(long i, long j);
void AM_setAdjacent(long i, long j);

#endif // ADJACENCY_MATRIX_H_
