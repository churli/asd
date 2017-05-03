#include "AVLTreeSet.h"
//

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
