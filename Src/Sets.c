#include "Sets.h"

SetElement * SetElement_new(long serial, long degree)
{
  SetElement * newElement = malloc(sizeof(SetElement));
  newElement->serial = serial;
  newElement->degree = degree;
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

// int AVLTreeSet_compare(SetElement * a, SetElement * b)
// {
//   // Returns n<0 if a<b, n=0 if a==b, n>0 if a>b
//   int res;
//   res = b->degree - a->degree; // We want descending degree, i.e. deg(a)>deg(b) ==> a<b
//   if (res == 0)
//   {
//     res = a->serial - b->serial; // We want ascending serial, i.e. ser(a)<ser(b) ==> a<b
//   }
//   return res;
// }

// int AVLTreeSet_getHeight(SetElement * node)
// {
//   if (node == NULL)
//   {
//     return -1;
//   }
//   else
//   {
//     return node->h;
//   }
// }

// SetElement * AVLTreeSet_leftRotation(SetElement * node)
// {
//   SetElement * repl = node->right;
//   node->right = repl->left;
//   repl->left = node;
//   node->h = max(AVLTreeSet_getHeight(node->left), AVLTreeSet_getHeight(node->right)) + 1;
//   repl->h = max(AVLTreeSet_getHeight(repl->left), AVLTreeSet_getHeight(repl->right)) + 1;
//   return repl;
// }

// SetElement * AVLTreeSet_rightRotation(SetElement * node)
// {
//   SetElement * repl = node->left;
//   node->left = repl->right;
//   repl->right = node;
//   node->h = max(AVLTreeSet_getHeight(node->left), AVLTreeSet_getHeight(node->right)) + 1;
//   repl->h = max(AVLTreeSet_getHeight(repl->left), AVLTreeSet_getHeight(repl->right)) + 1;
//   return repl;
// }

// SetElement * AVLTreeSet_add(SetElement * node, SetElement * newElement, SetElement ** firstInSubtreePtr, int * added)
// {
//   // Puts added = 1 if added, added = 2 if reactivated.
//   if (node == NULL)
//   {
//     *added = 1;
//     *firstInSubtreePtr = newElement;
//     return newElement;
//   }
//   else
//   {
//     long comparison = AVLTreeSet_compare(newElement, node);
//     if (comparison < 0)
//     {
//       // Left insertion
//       newElement->next = node; // Keeping the list ordering
//       node->left = AVLTreeSet_add(node->left, newElement, firstInSubtreePtr, added);
//       if (AVLTreeSet_getHeight(node->left) - AVLTreeSet_getHeight(node->right) == 2)
//       {
//         if (AVLTreeSet_compare(newElement, node->left) > 0)
//         {
//           node->left = AVLTreeSet_leftRotation(node->left);
//         }
//         node = AVLTreeSet_rightRotation(node);
//       }
//     }
//     else if (comparison > 0)
//     {
//       // Right insertion, here ignore the firstInSubtree coming from recursion

//       SetElement * firstInSubtreeOrig = *firstInSubtreePtr;
//       node->right = AVLTreeSet_add(node->right, newElement, firstInSubtreePtr, added);
//       node->next = *firstInSubtreePtr; // Putting the first of the right subtree as next.
//       *firstInSubtreePtr = firstInSubtreeOrig;

//       if (AVLTreeSet_getHeight(node->right) - AVLTreeSet_getHeight(node->left) == 2)
//       {
//         if (AVLTreeSet_compare(newElement, node->right) < 0)
//         {
//           node->right = AVLTreeSet_rightRotation(node->right);
//         }
//         node = AVLTreeSet_leftRotation(node);
//       }
//     }
//     else
//     {
//       if (node->deleted)
//       {
//         *added = 2;
//         node->deleted = 0; // reactivating a deleted node
//       }
//       else
//       {
//         LOG("WARNING: Trying to add an element which is already present in set!");
//       }
//       return node; // Important to return here not to increment height.
//     }
//   }
//   node->h = max(AVLTreeSet_getHeight(node->left), AVLTreeSet_getHeight(node->right)) + 1;
//   return node;
// }

// void Set_addAVL(Set * set, SetElement * newElement)
// {
//   int added = 0;
//   SetElement ** firstInTreePtr = &(set->first);
//   set->root = AVLTreeSet_add(set->root, newElement, firstInTreePtr, &added);
//   if (added == 1)
//   {
//     ++(set->size);
//   }
//   else if (added == 2)
//   {
//     --(set->deleted);
//   }
// }

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

// SetElement * AVLTreeSet_search(SetElement * node, SetElement * target) // Note that target is not the exact same object, but just another one with same serial&degree
// {
//   // Returns the match, if any
//   if (node == NULL)
//   {
//     return NULL;
//   }
//   else
//   {
//     long comparison = AVLTreeSet_compare(target, node);
//     if (comparison < 0)
//     {
//       return AVLTreeSet_search(node->left, target);
//     }void Set_clearDeletedAndRebuild(Set * set)
//     else if (comparison > 0)
//     {
//       return AVLTreeSet_search(node->right, target);
//     }
//     else
//     {
//       return node;
//     }
//   }
// }

// void Set_clearDeletedAndRebuild(Set * set)
// {
//   SetElement * cur = set->first;
//   while (cur != NULL)
//   {
    
//   }
// }

// void Set_removeAVL(Set * set, SetElement * target)
// {
//   if (target != NULL)
//   {
//     SetElement * result = AVLTreeSet_search(set->root, target);
//     if (result != NULL)
//     {
//       result->deleted = 1;
//       --(set->size);
//       ++(set->deleted);
//       if (set->size < 2*(set->deleted))
//       {
//         // If deleted nodes account for more than half of the tree, purge them

//       }
//     }
//   }
// }

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

// eof
