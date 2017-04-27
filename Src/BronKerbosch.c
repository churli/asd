#include "BronKerbosch.h"

SetElement * SetElement_new(long serial, long degree)
{
  SetElement * newElement = malloc(sizeof(SetElement));
  newElement->serial = serial;
  newElement->degree = degree;
  newElement->next = NULL;
  return newElement;
}

Set * Set_new()
{
  Set * new = malloc(sizeof(Set));
  new->size = 0;
  new->first = NULL;
  new->last = NULL;
  return new;
}

bool Set_isEmpty(Set * set)
{
  return set->size == 0;
}

void Set_add(Set * set, SetElement * newElement)
{
  // Set is ordered by decreasing degree of elements (and secondly by increasing serial, otherwise intersection breaks).
  if (set->first == NULL)
  {
    // Empty set?
    set->first = newElement;
    set->last = newElement;
  }
  else if (set->first->degree == newElement->degree
           && set->first->serial > newElement->serial) // Degrees are equal, first serial > new serial
  {
    // In this case new should be first
    newElement->next = set->first;
    set->first = newElement;
  }
  else if (set->first->degree < newElement->degree) // First degree < new degree
  {
    // Should new be first?
    newElement->next = set->first;
    set->first = newElement;
  }
  else // Degrees equal and first serial <= new serial OR first degree > new degree
  {
    // So new is surely not first.
    SetElement * cur = set->first;
    // Loop until we find the right place for the new element to go. Stop right before.
    while (cur->next != NULL 
           && (cur->next->degree > newElement->degree
               || (cur->next->degree == newElement->degree && cur->next->serial < newElement->serial)
               )
           )
    {
      cur = cur->next;
    }
    // And insert the new element between cur and next.
    newElement->next = cur->next;
    cur->next = newElement;

    // If new element is the last one, update last pointer
    if (newElement->next == NULL)
    {
      set->last = newElement;
    }
  }

  ++(set->size);
}

void Set_remove(Set * set, long serial)
{
  // Remove element by serial from set.

  if (set->first == NULL)
  {
    // Empty set?
    return;
  }
  else if (set->first->serial == serial)
  {
    SetElement * target = set->first;
    set->first = target->next;
    free(target);
    if (set->first == NULL)
    {
      set->last = NULL;
    }
  }
  else
  {
    // So target is surely not first.
    SetElement * cur = set->first;
    // Loop until we find the target. Stop right before.
    while (cur->next != NULL && cur->next->serial != serial)
    {
      cur = cur->next;
    }
    SetElement * target = cur->next;
    if (target == NULL)
    {
      cur->next = NULL;
      //LOG("WARNING: Couldn't find serial %ld in set.", serial);
    }
    else
    {
      cur->next = target->next;
      free(target);
    }
    
    // If currently reached element is the last one, update last pointer
    if (cur->next == NULL)
    {
      set->last = cur;
    }
  }

  --(set->size);
}

void Set_clear(Set * set)
{
  SetElement * cur = set->first;
  SetElement * todel = NULL;
  while(cur != NULL)
  {
    todel = cur;
    cur = cur->next;
    free(todel);
  }
  set->size = 0; //just for consistency
  set->first = NULL;
  set->last = NULL;
  // Eventually also free the Set object
  free(set);
}

void Set_print(Set * set, char * setName)
{
  printf("SET %s (%ld): ", setName, set->size);
  SetElement * cur = set->first;
  while(cur != NULL)
  {
    printf("%ld ", cur->serial);
    cur = cur->next;
  }
  printf("\n");
}

void Set_append(Set * set, SetElement * newElement)
{
  // NOTE: this is UNSAFE: it requires elements to be added following the proper order.
  if (set->last == NULL) // Set is empty
  {
    set->first = newElement;
    set->last = newElement;
  }
  else
  {
    set->last->next = newElement;
    set->last = newElement;
  }
  ++(set->size);
}

Set * Set_union(Set * a, Set * b)
{
  if (a == NULL || b == NULL)
    return NULL;
  Set * c = Set_new();
  SetElement * cura = a->first;
  SetElement * curb = b->first;
  SetElement * toadd;
  while (cura != NULL || curb != NULL)
  {
    if (cura == NULL)
    {
      toadd = SetElement_new(curb->serial, curb->degree); // copy values into new element
      Set_append(c, toadd);
      curb = curb->next;
    }
    else if (curb == NULL)
    {
      toadd = SetElement_new(cura->serial, cura->degree);
      Set_append(c, toadd);
      cura = cura->next;
    }
    else if (cura->degree > curb->degree)
    {
      toadd = SetElement_new(cura->serial, cura->degree);
      Set_append(c, toadd);
      cura = cura->next;
    }
    else if (cura->degree == curb->degree
             && cura->serial < curb->serial)
    {
      toadd = SetElement_new(cura->serial, cura->degree);
      Set_append(c, toadd);
      cura = cura->next;
    }
    else
    {
      toadd = SetElement_new(curb->serial, curb->degree);
      Set_append(c, toadd);
      curb = curb->next; 
    }
  }
  return c;
}

Set * Set_intersection(Set * a, Set * b)
{
  if (a == NULL || b == NULL)
    return NULL;
  Set * c = Set_new();
  SetElement * cura = a->first;
  SetElement * curb = b->first;
  SetElement * toadd;
  while (cura != NULL && curb != NULL)
  {
    //LOG("SET INTERSECTION: (%ld,%ld), (%ld,%ld)", cura->serial, cura->degree, curb->serial, curb->degree); //debug
    // NOTE: Note that we want to intersect by serial number, but if serial numbers are equal, then also degree will be equal.
    if (cura->degree > curb->degree)
    {
      cura = cura->next;
    }
    else if (cura->degree == curb->degree)
    {
      if (cura->serial < curb->serial)
      {
        cura = cura->next;
      }
      else if (cura->serial == curb->serial)
      {
        //LOG("SET INTERSECTION: good, adding to intersection"); //debug
        toadd = SetElement_new(cura->serial, cura->degree);
        Set_append(c, toadd);
        cura = cura->next;
        curb = curb->next;
      }
      else
      {
        curb = curb->next;
      }
    }
    else
    {
      curb = curb->next;
    }
  }
  return c;
}

Set * Set_difference(Set * a, Set * b)
{
  // Returns a new set C = A \ B.
  if (a == NULL || b == NULL)
    return NULL;
  Set * c = Set_new();
  SetElement * cura = a->first;
  SetElement * curb = b->first;
  SetElement * toadd;
  while (cura != NULL)
  {
    if (curb == NULL || cura->degree > curb->degree)
    {
      toadd = SetElement_new(cura->serial, cura->degree);
      Set_append(c, toadd);
      ++(c->size);
      cura = cura->next;
    }
    else if (cura->degree == curb->degree)
    {
      if (cura->serial < curb->serial)
      {
        toadd = SetElement_new(cura->serial, cura->degree);
        Set_append(c, toadd);
        cura = cura->next;
      }
      else if (cura->serial == curb->serial)
      {
        cura = cura->next;
        curb = curb->next;
      }
      else
      {
        curb = curb->next;
      }
    }
    else
    {
      curb = curb->next;
    }
  }
  return c;
}

SetElement * Set_getFirstInUnion(Set * a, Set * b)
{
  if (a == NULL || b == NULL)
    return NULL;
  SetElement * cura = a->first;
  SetElement * curb = b->first;
  SetElement * firstInUnion = NULL;

  if (cura == NULL && curb == NULL)
  {
    return NULL;
  }
  else if (cura == NULL)
  {
    firstInUnion = SetElement_new(curb->serial, curb->degree);
  }
  else if (curb == NULL)
  {
    firstInUnion = SetElement_new(cura->serial, cura->degree);
  }
  else if (cura->degree > curb->degree)
  {
    firstInUnion = SetElement_new(cura->serial, cura->degree);
  }
  else if (cura->degree == curb->degree
           && cura->serial < curb->serial)
  {
    firstInUnion = SetElement_new(cura->serial, cura->degree);
  }
  else
  {
    firstInUnion = SetElement_new(curb->serial, curb->degree);
  }

  return firstInUnion;
}

Set * getNeighbours(long serial)
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
      Set_clear(vNeighbours);

      bronKerbosch(newR, newP, newX);
      
      Set_clear(newR);
      Set_clear(newP);
      Set_clear(newX);

      Set_remove(P, vOrig->serial);
      Set_add(X, SetElement_new(vOrig->serial, vOrig->degree));
    }
    Set_clear(uNeighbours);
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
  LOG("AM_getNumGraphElements() = %ld", numElements);
  for (long serial = 0; serial < numElements; ++serial)
  {
    Set_add(P, SetElement_new(serial, DV_getDegree(serial)));
  }

  bronKerbosch(R, P, X);
  LOG("FINAL MAX CLIQUE: size %ld", _maxCliqueSize);
}

// eof
