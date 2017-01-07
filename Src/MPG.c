#include "MPG.h"

/* MPG construction */
MpgElem* MPG_getNewElem(Atom* a0, Atom* a1) {
  MpgElem* new = malloc(sizeof(MpgElem));
  new->atom[0] = a0;
  new->atom[1] = a1;
  new->adj = NULL;
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
  Graph cur1 = g1;
  Graph cur2 = g2;
  MpgElem *curElem, *newElem;
  Atom *a1, *a2;
  while (cur1 != NULL) {
  a1 = cur1->atom;
  printf("a1: (%s,%d)\n", ELEMENTS[a1->element], a1->secStructure); //debug
  while (cur2 != NULL) {
    a2 = cur2->atom;
    printf("a2: (%s,%d)\n", ELEMENTS[a2->element], a2->secStructure); //debug
    //here we have to carefully choose only the possible matches
    if (MPG_areAtomsCompatible(a1,a2)) {
      printf("(%s,%d) : (%s,%d)\n", ELEMENTS[a1->element], a1->secStructure, 
          ELEMENTS[a2->element], a2->secStructure); //debug
      newElem = MPG_getNewElem(a1, a2);
      if (*mpg == NULL) {
        *mpg = newElem;
        curElem = newElem;
      }
      else {
        curElem->next = newElem;
        curElem = newElem;
      }
    }
    cur2 = cur2->next;
  }
  cur1 = cur1->next;
  cur2 = g2; //don't forget to reset cur2 at each cur1 iteration!
  }
}

void MPG_computeEdges(MPG *mpg) {
    MpgElem *e1, *e2;
    if (*mpg == NULL || (*mpg)->next == NULL)
        return;
    e1 = *mpg;
    while (e1 != NULL) {
        e2 = e1->next;
        while (e2 != NULL) {
            //check if edge
            
            //in case set edge
            e2 = e2->next;
        }
        e1 = e1->next;
    }
}

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
  MPG_buildSet(mpg, g1, g2);
  MPG_computeEdges(mpg);
  return mpg;
}

//eof
