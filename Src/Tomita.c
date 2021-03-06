#include "Tomita.h"

void Tomita_expand(Set * R, Set * Q, Set * Qmax, long * No)
{
  while (R->size > 0)
  {
    SetElement * p = R->last;

    if (Q->size + No[p->serial] > Qmax->size)
    {
      SetElement * p1 = SetElement_new(p->serial, p->degree);
      Set_append(Q, p1); // Ordering doesn't matter in Q

      Set * Rp = ColorSet_intersectWithNeighboursOf(No, R, p->serial);
      
      if (Rp->size > 0)
      {
        long * newNo = calloc(AM_getNumGraphElements(), sizeof(long *));
        for (long i = 0; i < AM_getNumGraphElements(); ++i)
        {
          newNo[i] = 1;
        }
        Tomita_numberSort(Rp, newNo);
        Tomita_expand(Rp, Q, Qmax, newNo);
        free(newNo);
      }
      else if (Q->size > Qmax->size)
      {
        onNewClique(Q->size);
        Set * Qcopy = Set_copy(Q);
        Set_clear(Qmax);
        *Qmax = *Qcopy;
        free(Qcopy);
      }
      Set_removeLast(Q);
      Set_free(Rp);
    }
    else
    {
      return;
    }
    Set_removeLast(R);
  }
}

void Tomita_numberSort(Set * R, long * No)
{
  // NUMBER
  long k = 0;
  long maxno = 1;
  // Allocate a Set ** C somewhere
  Set ** C = calloc(AM_getNumGraphElements(), sizeof(Set *));
  C[1] = Set_new();
  while (R->size > 0)
  {
    SetElement * p = R->first;
    k = 1;
    while (ColorSet_getFirstInIntersectionWithNeighboursOf(No, C[k], p->serial) != NULL)
    {
      ++k;
    }
    if (k > maxno)
    {
      maxno = k;
      C[maxno] = Set_new();
    }
    No[p->serial] = k;
    ColorSet_add(No, C[k], SetElement_new(p->serial, p->degree));
    Set_remove(R, p->serial); // This is cheap since p is always the 1st in set.
  }
  // SORT
  for (k = 1; k <= maxno; ++k)
  {
    SetElement * cur = C[k]->first;
    while (cur != NULL)
    {
      Set_append(R, cur);
      cur = cur->next;
    }
    free(C[k]);
  }
  free(C);
}

void startTomita()
{
  setMaxCliqueSize(0);
  Set * R = Set_new();
  Set * Q = Set_new();
  Set * Qmax = Set_new();

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
  free(degreesDistribution);

  // Init R to the entire MPG vertex set
  LOG("R set initial sorting");
  for (long serial = 0; serial < numElements; ++serial)
  {
    Set_add(R, SetElement_new(serial, DV_getDegree(serial)));
  }

  LOG("Initial Tomita numbering");
  long * No = calloc(numElements, sizeof(long *));
  SetElement * cur = R->first;
  SetElement * prev = NULL;
  for (long i = 1; i <= DV_getMaxDegree() && cur != NULL; ++i)
  {
    No[cur->serial] = i;
    prev = cur;
    cur = cur->next;
  }
  long termColor = DV_getMaxDegree() + 1;
  
  while (cur != NULL)
  {
    No[cur->serial] = termColor;
    cur = cur->next; 
  }

  LOG("Start Tomita maximum clique finding");

  Tomita_expand(R, Q, Qmax, No);
  LOG("FINAL MAX CLIQUE: size %ld", getMaxCliqueSize());
  // Set_print(Qmax, "Qmax"); // debug
  LOG("Cleaning up...");
  Set_free(R);
  Set_free(Q);
  Set_free(Qmax);
  free(No);
  AM_free();
  DV_free();
}

// eof
