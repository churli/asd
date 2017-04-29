#include "BronKerbosch.h"

void allocateNeighboursSetArray()
{
  long numElements = AM_getNumGraphElements();
  _neighboursSetArray = calloc(numElements, sizeof(Set*));
}

Set * computeNeighbours(long serial)
{
  // Returns the open neighborhood of serial (i.e. serial excluded)
  long N = AM_getNumGraphElements();
  Set * neigh = Set_new();
  long i;
  for (i = 0; i < serial; ++i)
  {
    if (AM_getIfAdjacent(i, serial))
    {
      Set_add(neigh, SetElement_new(i, DV_getDegree(i)));
    }
  }
  for (i = serial + 1; i < N; ++i)
  {
    if (AM_getIfAdjacent(serial, i))
    {
      Set_add(neigh, SetElement_new(i, DV_getDegree(i)));
    } 
  }
  return neigh;
}

Set * getNeighbours(long serial)
{
  return _neighboursSetArray[serial];
}

void onNewClique(long newCliqueSize)
{
  if (newCliqueSize > _maxCliqueSize)
  {
    _maxCliqueSize = newCliqueSize;
    LOG("NEW CLIQUE: size %ld", newCliqueSize);
  }
}

void bronKerbosch(Set * R, Set * P, Set * X)
{
  // This should be called with non-NULL arguments. Sets can be empty: they just have to have a 0 size.

  //LOG("bronKerbosch: R=%ld, P=%ld, X=%ld", R->size, P->size, X->size); //debug
  //Set_print(R, "R"); //debug
  //Set_print(P, "P"); //debug

  if (Set_isEmpty(P) && Set_isEmpty(X))
  {
    onNewClique(R->size);
    // Set_print(R, "R"); // debug
    return;
  }
  else
  {
    Set * uNeighbours = getNeighbours(Set_getFirstInUnion(P, X)->serial);
    Set * C = Set_difference(P, uNeighbours);
    for (SetElement * vOrig = C->first; vOrig != NULL; vOrig = vOrig->next)
    {
      SetElement * v = SetElement_new(vOrig->serial, vOrig->degree);
      //LOG("vSerial = %ld", v->serial); //debug
      Set * vSingleton = Set_new();
      Set_add(vSingleton, v); 
      //LOG("vSingleton->size = %ld, vSingleton->first->serial = %ld", vSingleton->size, vSingleton->first->serial); //debug
      Set * vNeighbours = getNeighbours(v->serial);
      //LOG("vNeighbours->size = %d", vNeighbours->size); //debug
      //Set_print(vNeighbours, "vNeighbours"); //debug

      Set * newR = Set_union(R, vSingleton);
      //LOG("newR->size = %ld", newR->size); //debug
      Set * newP = Set_intersection(P, vNeighbours);
      Set * newX = Set_intersection(X, vNeighbours);

      Set_clear(vSingleton);
      //Set_clear(vNeighbours);

      bronKerbosch(newR, newP, newX);
      
      Set_clear(newR);
      Set_clear(newP);
      Set_clear(newX);

      Set_remove(P, vOrig->serial);
      Set_add(X, SetElement_new(vOrig->serial, vOrig->degree));
    }
    //Set_clear(uNeighbours);
    Set_clear(C);
  }
}

void startBronKerbosch()
{
  _maxCliqueSize = 0;
  Set * R = Set_new();
  Set * P = Set_new();
  Set * X = Set_new();

  // Init P to the entire MPG vertex set
  long numElements = AM_getNumGraphElements();
  LOG("|MPG| = %ld", numElements);
  long maxDegree = DV_getMaxDegree();
  LOG("MaxDegree(MPG) = %ld", maxDegree);

  long * degreesDistribution = calloc(maxDegree+1, sizeof(long));
  for (long serial = 0; serial < numElements; ++serial)
  {
    long deg = DV_getDegree(serial);
    ++*(degreesDistribution+deg);
  }
  long mpcs = maxDegree;
  long nodesWithGeqDegree = degreesDistribution[mpcs];
  while (mpcs > nodesWithGeqDegree && mpcs > 0)
  {
    //LOG("degreesDistribution[%ld] = %ld", mpcs, nodesWithGeqDegree); // debug
    --mpcs;
    nodesWithGeqDegree += degreesDistribution[mpcs];
  }
  LOG("MaxPossibleCliqueSize(MPG) = %ld", mpcs);

  allocateNeighboursSetArray();
  LOG("Start precomputing neighbours");
  for (long serial = 0; serial < numElements; ++serial)
  {
    Set_add(P, SetElement_new(serial, DV_getDegree(serial)));
    Set * neighbours = computeNeighbours(serial);
    _neighboursSetArray[serial] = neighbours;
  }

  LOG("Start Bron-Kerbosch clique finding");

  bronKerbosch(R, P, X);
  LOG("FINAL MAX CLIQUE: size %ld", _maxCliqueSize);
  LOG("Cleaning up...");
  for (long serial = 0; serial < numElements; ++serial)
  {
    free(_neighboursSetArray[serial]);
  }
  free(_neighboursSetArray);
}

// eof
