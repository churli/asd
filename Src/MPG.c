#include "MPG.h"

// Methods
void AdjMpgElemList_append(AdjMpgElemList this, AdjMpgElem *new) {
  if (this == NULL || new == NULL) {
    LOG("FATAL: You cannot pass a NULL argument to AdjMpgElemList_append. Exiting...");
    exit(666);
  }
  while (this->next != NULL)
    this = this->next;
  this->next = new;
}

void MpgElem_addAdjElem(MpgElem *this, MpgElem *new) {
  AdjMpgElem* newAdj = malloc(sizeof(AdjMpgElem));
  newAdj->mpgElem = new;
  newAdj->next = NULL;
  if (this->adj == NULL)
    //it means that this is the first adj elem
    this->adj = newAdj;
  else
    AdjMpgElemList_append(this->adj, newAdj);
}

//todo I should use an adjacency matrix instead of list of adjacent nodes...(faster access!)

/* MPG construction */
MpgElem* MPG_getNewElem(Atom* a0, Atom* a1) {
  MpgElem* new = malloc(sizeof(MpgElem));
  new->atom[0] = a0;
  new->atom[1] = a1;
  new->adj = NULL;
  new->next = NULL;
  return new;
}

bool MPG_areAtomsCompatible(Atom* a, Atom* b) {
  // printf("(%s,%d) : (%s,%d)\n", ELEMENTS[a->element], a->secStructure, ELEMENTS[b->element], b->secStructure); //debug
  return (
      a->element == b->element
      && a->secStructure == b->secStructure
      );
}

void MPG_buildSet(MPG *mpg, Graph g1, Graph g2) {
  int elemCounter = 0;
  Graph cur1 = g1;
  Graph cur2 = g2;
  MpgElem *curElem = NULL, *newElem = NULL;
  Atom *a1, *a2;
  while (cur1 != NULL) {
  a1 = cur1->atom;
  // printf("a1: (%s,%d)\n", ELEMENTS[a1->element], a1->secStructure); //debug
  while (cur2 != NULL) {
    a2 = cur2->atom;
    // printf("a2: (%s,%d)\n", ELEMENTS[a2->element], a2->secStructure); //debug
    //here we have to carefully choose only the possible matches
    if (MPG_areAtomsCompatible(a1, a2)) {
      // printf("(%s,%d) : (%s,%d)\n", ELEMENTS[a1->element], a1->secStructure, 
      //     ELEMENTS[a2->element], a2->secStructure); //debug
      newElem = MPG_getNewElem(a1, a2);
      ++elemCounter;
      // if (*mpg == NULL) {
      //   *mpg = newElem;
      //   curElem = newElem;
      // }
      // else {
      //   curElem->next = newElem;
      //   curElem = newElem;
      // }
      MPG_addElementAndComputeEdges(mpg, newElem); //trying the new "incremental" method
    }
    cur2 = cur2->next;
  }
  cur1 = cur1->next;
  cur2 = g2; //don't forget to reset cur2 at each cur1 iteration!
  }
  LOG("MPG has %d elements.", elemCounter);
}

void MPG_addElementAndComputeEdges(MPG *mpg, MpgElem *newElem) {
  // The idea here is to compute the edges AND perform Bron-Kerbosch (or other MAX-clique-finding algo)
  // (mean)while we are filling the MPG.

  int elemCounter = 0; //debug
  // Add and compute edges
  if (*mpg == NULL) {
    *mpg = newElem;
    return;
  }
  //else go on
  MpgElem* curElem = *mpg;
  while (curElem->next != NULL) {
    //compute edges here
    if (Atom_getBondType(newElem->atom[0], curElem->atom[0]) 
        == Atom_getBondType(newElem->atom[1], curElem->atom[1]))
    {
      //if eligible for edge in MPG, add the edjes to both objects
      MpgElem_addAdjElem(curElem, newElem);
      MpgElem_addAdjElem(newElem, curElem);
    }
      
    //
    elemCounter++; //debug
    curElem = curElem->next;
  }
  //last element currently in MPG
  if (Atom_getBondType(newElem->atom[0], curElem->atom[0]) 
        == Atom_getBondType(newElem->atom[1], curElem->atom[1]))
  {
    //if eligible for edge in MPG, add the edjes to both objects
    MpgElem_addAdjElem(curElem, newElem);
    MpgElem_addAdjElem(newElem, curElem);
  }
  elemCounter++; //debug
  //finally append newElem to MPG
  curElem->next = newElem;
  LOG("Added %d-th element to MPG", elemCounter);
}

// void MPG_computeEdges(MPG *mpg) {
//     MpgElem *e1, *e2;
//     if (*mpg == NULL || (*mpg)->next == NULL)
//         return;
//     e1 = *mpg;
//     //
//     int i = 0; //debug
//     int j = 0; //debug
//     while (e1 != NULL) {
//       LOG("DEBUG: MPG computing edge of (%d,%d)", i, j);
//       j = 0; //debug
//       e2 = e1->next;
//       while (e2 != NULL) {
//         //check if edge
//         //in case set edge
//         //todo
//         e2 = e2->next;
//         ++j;
//       }
//       e1 = e1->next;
//       ++i;
//     }
// }

MPG* MPG_buildMPG(Protein* p1, Protein* p2) {
  if (p1 == NULL || p2 == NULL) {
    printf("FATAL: Proteins cannot be NULL!");
    exit(666);
    return NULL;
  }
  Graph g1 = p1->graph;
  Graph g2 = p2->graph;
  if (g1 == NULL || g2 == NULL) {
    printf("FATAL: Graphs cannot initially be NULL!");
    exit(666);
    return NULL;
  }
  MPG* mpg = malloc(sizeof(MPG));
  *mpg = NULL;
  LOG("Starting MPG_buildSet...");
  MPG_buildSet(mpg, g1, g2);
  // LOG("Starting MPG_computeEdges...");
  // MPG_computeEdges(mpg);
  return mpg;
}

//eof
