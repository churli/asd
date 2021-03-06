#include "AdjacencyMatrix.h"

// Mem allocation (initialization)
void AM_initialize(long numGraphElements)
{
  _numGraphElements = numGraphElements;
  unsigned long adjVectorItems = (numGraphElements * (numGraphElements - 1)) / 2;
  unsigned long vectorLength = ((adjVectorItems/8) + 1);
  _adjacencyVector = calloc(vectorLength, sizeof(char));
  if (_adjacencyVector == NULL)
  {
    LOG("Couldn't allocate the adjacency vector: %lu bytes", vectorLength);
    exit(666);
  }
}

void AM_free()
{
  free(_adjacencyVector);
}

long AM_getNumGraphElements()
{
  return _numGraphElements;
}

// Indexing (0-based)
long AM_getIndex(long i, long j)
{
  long index = 0;
  long k = _numGraphElements;

  index += i*(k) - ((i+1)*(i)/2); // The second term here is sum of all integers < (i)

  index += j - i - 1;
  return index;
}

long AM_getCharIndex(long index)
{
  return index / 8;
}

short AM_getSubBitIndex(long index)
{
  return index % 8;
}

// Getting bits
bool AM_getBit(char byte, short i)
{
  return (byte >> i) & 1;
}

// Setting bits
void AM_setBit(char* bytePtr, short i)
{
  // Set the i-th bit of *bytePtr to 1.
  *bytePtr |= (1 << i);
}

// High level operations, set * get
bool AM_getIfAdjacent(long i, long j)
{
  long index = AM_getIndex(i, j);
  long cIndex = AM_getCharIndex(index);
  short sbIndex = AM_getSubBitIndex(index);

  char cItem = *(_adjacencyVector + cIndex);
  return AM_getBit(cItem, sbIndex);
}

void AM_setAdjacent(long i, long j)
{
  long index = AM_getIndex(i, j);
  long cIndex = AM_getCharIndex(index);
  short sbIndex = AM_getSubBitIndex(index);

  AM_setBit(_adjacencyVector + cIndex, sbIndex);
}

void DV_initialize(long numGraphElements)
{
  // Allocating mem
  _degreeVector = calloc(numGraphElements, sizeof(long));
  if (_degreeVector == NULL)
  {
    LOG("Couldn't allocate the degree vector: %lu bytes", numGraphElements * sizeof(long));
    exit(666);
  }
}

void DV_free()
{
  free(_degreeVector);
}

void DV_setDegree(long serial, long degree)
{
  *(_degreeVector + serial) = degree;
  if (degree > _maxDegree)
  {
    _maxDegree = degree;
  }
}

void DV_increaseDegree(long serial)
{
  if (++(*(_degreeVector + serial)) > _maxDegree)
  {
    _maxDegree = *(_degreeVector + serial);
  }
}

long DV_getDegree(long serial)
{
  return *(_degreeVector + serial);
}

long DV_getMaxDegree()
{
  return _maxDegree;
}

// eof
