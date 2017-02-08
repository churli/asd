#include "AdjacencyMatrix.h"

// Mem allocation (initialization)
void AM_initialize(long numGraphElements)
{
  _numGraphElements = numGraphElements;
  unsigned long adjVectorItems = (numGraphElements * (numGraphElements - 1)) / 2;
  unsigned long vectorLength = ((adjVectorItems/8) + 1);
  _adjacencyVector = malloc( vectorLength * sizeof(char));
  if (_adjacencyVector == NULL)
  {
    LOG("Couldn't allocate the adjacency vector: %lu bytes", vectorLength);
    exit(666);
  }
}

// Indexing
long AM_getIndex(long i, long j)
{
  long index = 0;
  long k = _numGraphElements;

  //for (long l = 0; i - 1; ++l)
  //  index += k - l - 1;
  index += i*(k-1) - ((i-1)*(i-2)/2);

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
  *bytePtr = *bytePtr | (1 << i);
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

// eof
