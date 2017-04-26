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

long VSet_getLength()
{
  return _vSetLength;
}

long VSet_getByteLength()
{
  return _vSetByteLength;
}

char * VSet_new()
{
  //char * vset = malloc(_vSetByteLength * sizeof(char));
  char * vset = calloc(_vSetByteLength, sizeof(char));
  // for (long i = 0; i < _vSetByteLength; ++i)
  // {
  //   *(vset + i) = 0;
  // }
  if (vset == NULL)
  {
    LOG("Couldn't allocate vset: %lu bytes", _vSetByteLength * sizeof(char));
    exit(666);
  }
  return vset;
}

bool VSet_isEmpty(char * vset)
{
  for (long i = 0; i < _vSetByteLength; ++i)
  {
    if (*(vset + i) != 0)
    {
      return FALSE;
    }
  }
  return TRUE;
}

long VSet_getSize(char * vset)
{
  long size = 0;
  for (long i = 0; i < _vSetByteLength; ++i)
  {
    char b = *(vset + i);
    if (b != 0)
    {
      for (int j = 0; j < 8; ++j)
      {
        size += (b >> j) & 1;
      }
    }
  }
  return size;
}

void VSet_add(char * vset, long newSerial)
{
  long byteIndex = VSet_byteIndex(newSerial);
  short bitIndex = VSet_bitIndex(newSerial);

  *(vset + byteIndex) = *(vset + byteIndex) | (1 << bitIndex);
}

int VSet_get(char * vset, long serial)
{
  long byteIndex = VSet_byteIndex(serial);
  short bitIndex = VSet_bitIndex(serial);

  return (*(vset + byteIndex) >> bitIndex) & 1;
}

void VSet_remove(char * vset, long serial)
{
  long byteIndex = VSet_byteIndex(serial);
  short bitIndex = VSet_bitIndex(serial);

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

long VSet_getFirstInUnion(char * a, char * b)
{
  char byte = 0;
  long i = 0;
  for (i = 0; i < _vSetByteLength; ++i)
  {
    if (*(a + i) != 0)
    {
      byte = *(a + i);
      break;
    }
    else if (*(b + i) != 0)
    {
      byte = *(b + i);
      break;
    }
  }

  int j = 0;
  while (!(byte & 1) && j<8)
  {
    byte = byte >> 1;
    ++j;
  }

  return (i*8) + j;
}

char * VSet_getNeighbours(long serial)
{
  // Returns the open neighborhood of serial (i.e. serial excluded)
  long N = AM_getNumGraphElements();
  char * neigh = VSet_new();
  long i;
  for (i = 0; i < serial; ++i)
  {
    if (AM_getIfAdjacent(i, serial))
    {
      VSet_add(neigh, i);
    }
  }
  for (i = serial + 1; i < N; ++i)
  {
    if (AM_getIfAdjacent(serial, i))
    {
      VSet_add(neigh, i);
    } 
  }
  return neigh;
}

// eof
