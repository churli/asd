#include "VectorSet.h"

long VSet_byteIndex(long serial)
{
  return serial / 8;
}

short VSet_bitIndex(long serial)
{
  return serial % 8;
}

void VSet_setLength(long length)
{
  _vSetLength = length;
  _vSetByteLength = (length / 8) + 1;
}

char * VSet_new()
{
  char * vset = malloc(_vSetByteLength * sizeof(char));
  for (long i = 0; i < _vSetByteLength; ++i)
  {
    *(vset + i) = 0;
  }
  return vset;
}

bool VSet_isEmpty(char * vset)
{
  for (long i = 0; i < _vSetByteLength; ++i)
  {
    if (*(vset + i) != 0)
      return FALSE;
  }
  return TRUE;
}

void VSet_add(char * vset, long newSerial)
{
  long byteIndex = VSet_byteIndex(newSerial);
  short bitIndex = VSet_bitIndex(newSerial);

  *(vset + byteIndex) |= 1 << bitIndex;
}

void VSet_remove(char * vset, long serial)
{
  long byteIndex = VSet_byteIndex(newSerial);
  short bitIndex = VSet_bitIndex(newSerial);

  *(vset + byteIndex) &= (~(1 << bitIndex)) & 0xFF; 
}

void VSet_clear(char * vset)
{
  free(vset);
}

char * VSet_union(char * a, char * b)
{
  char * c = VSet_new();
  for (long i = 0; i < _vSetByteLength; ++i)
  {
    *(c + i) = *(a + i) | *(b + i);
  }
  return c;
}

char * VSet_intersection(char * a, char * b)
{
  char * c = VSet_new();
  for (long i = 0; i < _vSetByteLength; ++i)
  {
    *(c + i) = *(a + i) & *(b + i);
  }
  return c;
}

char * VSet_difference(char * a, char * b)
{
  // returns c = a \ b
  char * c = VSet_new();
  for (long i = 0; i < _vSetByteLength; ++i)
  {
    *(c + i) = *(a + i) & ~(*(b + i));
  }
  return c;
}

long VSet_getFirstInUnion(char * a, char * b);

char * VSet_getNeighbours(long serial);

// eof
