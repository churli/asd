#ifndef ADJACENCY_MATRIX_H_
#define ADJACENCY_MATRIX_H_

#include <stdlib.h>
#include "Common.h"
#include "Logger.h"

static char * _adjacencyVector;
static long * _degreeVector;
static long _numGraphElements;
static long _maxDegree;

// Mem allocation (initialization)
void AM_initialize(long numGraphElements);

void AM_free();

long AM_getNumGraphElements();

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

void DV_initialize(long numGraphElements);

void DV_free();

void DV_setDegree(long serial, long degree);

void DV_increaseDegree(long serial);

long DV_getDegree(long serial);

long DV_getMaxDegree();

#endif // ADJACENCY_MATRIX_H_
