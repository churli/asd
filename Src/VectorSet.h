#ifndef VECTOR_SET_H_
#define VECTOR_SET_H_

#include <stdlib.h>
#include "Common.h"
#include "Logger.h"
#include "AdjacencyMatrix.h"

static long _vSetLength;
static long _vSetByteLength;

long VSet_byteIndex(long serial);

short VSet_bitIndex(long serial);

void VSet_setLength(long length);

long VSet_getLength();

long VSet_getByteLength();

char * VSet_new();

bool VSet_isEmpty(char * vset);

long VSet_getSize(char * vset);

void VSet_add(char * vset, long newSerial);

int VSet_get(char * vset, long serial);

void VSet_remove(char * vset, long serial);

void VSet_clear(char * vset);

char * VSet_union(char * a, char * b);

char * VSet_intersection(char * a, char * b);

char * VSet_difference(char * a, char * b);

long VSet_getFirstInUnion(char * a, char * b);

char * VSet_getNeighbours(long serial);

#endif // VECTOR_SET_H_
