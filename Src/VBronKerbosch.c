#include "VBronKerbosch.h"

void onVNewClique(long newCliqueSize)
{
  if (newCliqueSize > _maxCliqueSize)
  {
    _maxCliqueSize = newCliqueSize;
    LOG("NEW CLIQUE: size %ld", newCliqueSize);
  }
}

void vBronKerbosch(char * R, char * P, char * X)
{
  // This should be called with non-NULL arguments. Sets can be empty: they just have to have a 0 size.

  if (VSet_isEmpty(P) && VSet_isEmpty(X))
  {
    onVNewClique(VSet_getSize(R));
    return;
  }
  else
  {
    char * uNeighbours = VSet_getNeighbours(VSet_getFirstInUnion(P, X));
    char * C = VSet_difference(P, uNeighbours);
    long setLength = VSet_getLength();
    for (long v = 0; v < setLength; ++v)
    {
      if (VSet_get(C, v))
      {
        char * vSingleton = VSet_new();
        VSet_add(vSingleton, v); 
        char * vNeighbours = VSet_getNeighbours(v);

        char * newR = VSet_union(R, vSingleton);
        char * newP = VSet_intersection(P, vNeighbours);
        char * newX = VSet_intersection(X, vNeighbours);

        VSet_clear(vSingleton);
        VSet_clear(vNeighbours);

        vBronKerbosch(newR, newP, newX);
        
        VSet_clear(newR);
        VSet_clear(newP);
        VSet_clear(newX);

        VSet_remove(P, v);
        VSet_add(X, v);
      }
    }
    VSet_clear(uNeighbours);
    VSet_clear(C);
  }
}

void startVBronKerbosch()
{
  _maxCliqueSize = 0;

  // Init P to the entire MPG vertex set
  long numElements = AM_getNumGraphElements();
  LOG("AM_getNumGraphElements() = %ld", numElements);
  VSet_setLength(numElements);

  char * R = VSet_new();
  char * P = VSet_new();
  char * X = VSet_new();

  for (long serial = 0; serial < numElements; ++serial)
  {
    VSet_add(P, serial);
  }

  vBronKerbosch(R, P, X);
  LOG("FINAL MAX CLIQUE: size %ld", _maxCliqueSize);
}

// eof
