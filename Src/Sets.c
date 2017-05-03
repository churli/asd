#include "Sets.h"

SetElement * SetElement_new(long serial, long degree)
{
  SetElement * newElement = malloc(sizeof(SetElement));
  if (newElement == NULL)
  {
    LOG("FATAL: Couldn't allocate any more memory. Crashing.");
  }
  newElement->serial = serial;
  newElement->degree = degree;
  newElement->prev = NULL;
  newElement->next = NULL;
  // newElement->h = 0;
  // newElement->left = NULL;
  // newElement->right = NULL;
  // newElement->deleted = 0;
  return newElement;
}

Set * Set_new()
{
  Set * new = malloc(sizeof(Set));
  if (new == NULL)
  {
    LOG("FATAL: Couldn't allocate any more memory. Crashing.");
  }
  new->size = 0;
  new->first = NULL;
  new->last = NULL;
  // new->root = NULL;
  // new->deleted = 0;
  return new;
}

bool Set_isEmpty(Set * set)
{
  return set->size == 0;
}

long Set_compare(SetElement * a, SetElement * b)
{
  long res = 0;
  res = b->degree - a->degree;
  if (res == 0)
  {
    res = a->serial - b->serial;
  }
  return res;
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
  else if (Set_compare(newElement, set->first) < 0)
  {
    // In this case new should be first
    newElement->next = set->first;
    set->first->prev = newElement;
    set->first = newElement;
  }
  else // Degrees equal and first serial <= new serial OR first degree > new degree
  {
    // So new is surely not first.
    SetElement * cur = set->first;
    // Loop until we find the right place for the new element to go. Stop right before.
    while (cur->next != NULL 
           && Set_compare(newElement, cur->next) >= 0)
    {
      cur = cur->next;
    }
    // And insert the new element between cur and next. (if element is not already there)
    if (cur->next == NULL)
    {
      newElement->prev = cur;
      newElement->next = cur->next;
      cur->next = newElement;
    }
    else if (Set_compare(newElement, cur->next) != 0)
    {
      newElement->prev = cur;
      newElement->next = cur->next;
      cur->next->prev = newElement;
      cur->next = newElement; 
    }
    else
    {
      free(newElement);
      return;
    }

    // If new element is the last one, update last pointer
    if (newElement->next == NULL)
    {
      set->last = newElement;
    }
  }

  ++(set->size);
}

// void Set_addByAscendingSerial(Set * set, SetElement * newElement)
// {
//   if (set->first == NULL)
//   {
//     // Empty set?
//     set->first = newElement;
//     set->last = newElement;
//   }
//   else if (set->first->serial > newElement->serial)
//   {
//     // In this case new should be first
//     newElement->next = set->first;
//     set->first = newElement;
//   }
//   else
//   {
//     // So new is surely not first.
//     SetElement * cur = set->first;
//     // Loop until we find the right place for the new element to go. Stop right before.
//     while (cur->next != NULL 
//            && (cur->next->serial < newElement->serial)
//            )
//     {
//       cur = cur->next;
//     }
//     // And insert the new element between cur and next. (if element is not already there)
//     if (cur->next == NULL 
//         || newElement->serial != cur->next->serial)
//     {
//       newElement->next = cur->next;
//       cur->next = newElement;
//     }
//     else
//     {
//       free(newElement);
//       return;
//     }

//     // If new element is the last one, update last pointer
//     if (newElement->next == NULL)
//     {
//       set->last = newElement;
//     }
//   }

//   ++(set->size);
// }

long ColorSet_compare(long * No, SetElement * a, SetElement * b)
{
  long res = 0;
  res = No[a->serial] - No[b->serial];
  if (res == 0)
  {
    res = b->degree - a->degree;
    if (res == 0)
    {
      res = a->serial - b->serial;
    }
  }
  return res;
}

void ColorSet_add(long * No, Set * set, SetElement * newElement)
{

  // Set is ordered by increasing color of elements (and secondly by increasing serial, otherwise intersection breaks).
  if (set->first == NULL)
  {
    // Empty set?
    set->first = newElement;
    set->last = newElement;
  }
  else if (ColorSet_compare(No, newElement, set->first) < 0)
  {
    // In this case new should be first
    newElement->next = set->first;
    set->first->prev = newElement;
    set->first = newElement;
  }
  else
  {
    // So new is surely not first.
    SetElement * cur = set->first;
    // Loop until we find the right place for the new element to go. Stop right before.
    while (cur->next != NULL 
           && ColorSet_compare(No, newElement, cur->next) >= 0
           )
    {
      cur = cur->next;
    }
    // And insert the new element between cur and next. (if element is not already there)
    if (cur->next == NULL)
    {
      newElement->prev = cur;
      newElement->next = cur->next;
      cur->next = newElement;
    }
    else if (ColorSet_compare(No, newElement, cur->next) != 0)
    {
      newElement->prev = cur;
      newElement->next = cur->next;
      cur->next->prev = newElement;
      cur->next = newElement;
    }
    else
    {
      free(newElement);
      return;
    }

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
    else
    {
      set->first->prev = NULL;
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
      return;
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
    else
    {
      cur->next->prev = cur;
    }
  }

  --(set->size);
}

void Set_removeLast(Set * set)
{
  if (set == NULL || set->last == NULL)
  {
    return;
  }

  SetElement * target = set->last;
  set->last = target->prev;
  if (set->last != NULL)
  {
    set->last->next = NULL;
  }
  else
  {
    set->first = NULL;
  }
  free(target);

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
}

void Set_free(Set * set)
{
  Set_clear(set);
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

Set * Set_copy(Set * set)
{
  if (set == NULL)
  {
    return NULL;
  }
  Set * copy = Set_new();
  SetElement * cur = set->first;
  while (cur != NULL)
  {
    Set_append(copy, SetElement_new(cur->serial, cur->degree));
    cur = cur->next;
  }
  return copy;
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
    newElement->prev = set->last;
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
    else if (Set_compare(cura, curb) < 0)
    {
      toadd = SetElement_new(cura->serial, cura->degree);
      Set_append(c, toadd);
      cura = cura->next;
    }
    else if (Set_compare(cura, curb) == 0)
    {
      toadd = SetElement_new(cura->serial, cura->degree);
      Set_append(c, toadd);
      cura = cura->next;
      curb = curb->next; 
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
    if (Set_compare(cura, curb) < 0)
    {
      cura = cura->next;
    }
    else if (Set_compare(cura, curb) == 0)
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
  return c;
}

Set * ColorSet_intersection(long * No, Set * a, Set * b)
{
  // Sorted by ascending color
  if (a == NULL || b == NULL)
    return NULL;
  Set * c = Set_new();
  SetElement * cura = a->first;
  SetElement * curb = b->first;
  SetElement * toadd;
  while (cura != NULL && curb != NULL)
  {
    // NOTE: Note that we want to intersect by serial number, but if serial numbers are equal, then also color will be equal.
    long comparison = ColorSet_compare(No, cura, curb);
    if (comparison < 0)
    {
      cura = cura->next;
    }
    else if (comparison == 0)
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
    if (curb == NULL || Set_compare(cura, curb) < 0)
    {
      toadd = SetElement_new(cura->serial, cura->degree);
      Set_append(c, toadd);
      cura = cura->next;
    }
    else if (Set_compare(cura, curb) == 0)
    {
      cura = cura->next;
      curb = curb->next;
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
    // firstInUnion = SetElement_new(curb->serial, curb->degree);
    firstInUnion = curb; // UNSAFE
  }
  else if (curb == NULL)
  {
    // firstInUnion = SetElement_new(cura->serial, cura->degree);
    firstInUnion = cura; // UNSAFE
  }
  else if (Set_compare(cura, curb) <= 0)
  {
    // firstInUnion = SetElement_new(cura->serial, cura->degree);
    firstInUnion = cura; // UNSAFE
  }
  else
  {
    // firstInUnion = SetElement_new(curb->serial, curb->degree);
    firstInUnion = curb; // UNSAFE
  }

  return firstInUnion;
}

SetElement * Set_getFirstInIntersection(Set * a, Set * b)
{
  if (a == NULL || b == NULL)
    return NULL;
  SetElement * cura = a->first;
  SetElement * curb = b->first;

  while (cura != NULL && curb != NULL)
  {
    long comparison = Set_compare(cura, curb);
    if (comparison < 0)
    {
      cura = cura->next;
    }
    else if (comparison == 0)
    {
      // return SetElement_new(cura->serial, cura->degree);
      return cura; // UNSAFE
    }
    else
    {
      curb = curb->next;
    }
  }
  return NULL;
}

SetElement * ColorSet_getFirstInIntersection(long * No, Set * a, Set * b)
{
  if (a == NULL || b == NULL)
    return NULL;
  SetElement * cura = a->first;
  SetElement * curb = b->first;

  while (cura != NULL && curb != NULL)
  {
    if (ColorSet_compare(No, cura, curb) < 0)
    {
      cura = cura->next;
    }
    else if (ColorSet_compare(No, cura, curb) == 0)
    {
      // return SetElement_new(cura->serial, cura->degree);
      return cura; // UNSAFE
    }
    else
    {
      curb = curb->next;
    }
  }
  return NULL;
}

Set * ColorSet_intersectWithNeighboursOf(long * No, Set * a, long serial)
{
  if (a == NULL)
  {
    return NULL;
  }
  Set * c = Set_new();
  SetElement * cur = a->first;
  SetElement * toadd;
  while (cur != NULL)
  {
    bool isNeigbour;
    if (cur->serial < serial)
    {
      isNeigbour = AM_getIfAdjacent(cur->serial, serial);
    }
    else if (cur->serial > serial)
    {
      isNeigbour = AM_getIfAdjacent(serial, cur->serial);
    }
    else
    {
      isNeigbour = FALSE;
    }

    if (isNeigbour)
    {
      //LOG("SET INTERSECTION: good, adding to intersection"); //debug
      toadd = SetElement_new(cur->serial, cur->degree);
      Set_append(c, toadd);
      cur = cur->next;
    }
    else
    {
      cur = cur->next;
    }
  }
  return c;
}

SetElement * ColorSet_getFirstInIntersectionWithNeighboursOf(long * No, Set * a, long serial)
{
  if (a == NULL)
  {
    return NULL;
  }
  SetElement * cur = a->first;
  while (cur != NULL)
  {
    bool isNeigbour;
    if (cur->serial < serial)
    {
      isNeigbour = AM_getIfAdjacent(cur->serial, serial);
    }
    else if (cur->serial > serial)
    {
      isNeigbour = AM_getIfAdjacent(serial, cur->serial);
    }
    else
    {
      isNeigbour = FALSE;
    }

    if (isNeigbour)
    {
      //LOG("SET INTERSECTION: good, adding to intersection"); //debug
      return cur; // UNSAFE, its next doesn't belong necessarily to intersection
    }
    else
    {
      cur = cur->next;
    }
  }
  return NULL;
}

// Set * Set_reSortBySerial(SetElement * first)
// {
//   // returns the ptr to the new set representing the re-sorted tail
//   if (first == NULL)
//   {
//     return NULL;
//   }
  
//   Set * C = Set_new();
//   SetElement * cur = first;
//   SetElement * todel = NULL;
//   while (cur != NULL)
//   {
//     Set_addByAscendingSerial(C, SetElement_new(cur->serial, cur->degree));
//     todel = cur;
//     cur = cur->next;
//     free(todel);
//   }
//   return C;
// }

// eof
