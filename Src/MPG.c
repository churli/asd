#include "MPG.h"
#include "AdjacencyMatrix.h"

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
MpgElem* MPG_getNewElem(Atom* a0, Atom* a1, long serial) {
  MpgElem* new = malloc(sizeof(MpgElem));
  new->atom[0] = a0;
  new->atom[1] = a1;
  new->serial = serial;
  new->adj = NULL;
  new->next = NULL;
  return new;
}

bool MPG_areAtomsCompatible(Atom* a, Atom* b) {
  // printf("(%s,%d) : (%s,%d)\n", ELEMENTS[a->element], a->secStructure, ELEMENTS[b->element], b->secStructure); //debug
  return (
      a->element == b->element
      && a->secStructure == b->secStructure
      && a->amminoAcid == b->amminoAcid // Ammino-check! :)
      );
}

void MPG_buildSet(MPG *mpg, Graph g1, Graph g2) {
  long elemCounter = 0;
  Graph cur1 = g1;
  Graph cur2 = g2;
  MpgElem *curElem = NULL, *newElem = NULL;
  Atom *a1, *a2;
  while (cur1 != NULL) {
  a1 = cur1->atom;
  while (cur2 != NULL) {
    a2 = cur2->atom;
    if (MPG_areAtomsCompatible(a1, a2)) {
      ++elemCounter;
      newElem = MPG_getNewElem(a1, a2, elemCounter);
      MPG_addElement(mpg, newElem);
    }
    cur2 = cur2->next;
  }

  cur1 = cur1->next;
  cur2 = g2; //don't forget to reset cur2 at each cur1 iteration!
  }
  LOG("MPG has %d elements.", elemCounter);
}

void MPG_addElement(MPG *mpg, MpgElem *newElem)
{
  if (mpg->first == NULL)
  {
    mpg->first = newElem;
    mpg->last = newElem;
  }
  else
  {
    // Here assume MPG has been well-built, with last != NULL, always!
    mpg->last->next = newElem;
    mpg->last = newElem;
  }
  ++(mpg->elements);
}

void MPG_computeEdges(MPG *mpg)
{
  if (mpg == NULL || mpg->first == NULL)
  {
    LOG("FATAL: You cannot pass a NULL MPG or an MPG without first element to MPG_computeEdges. Exiting...");
    exit(666);
    return;
  }
  // else go on!
  MpgElem* curElem = mpg->first;
  while (curElem->next != NULL)
  {
    curElem = curElem->next; // start from 2nd elem, since on 1 elem we don't have any edge.
    for (MpgElem* otherElem = mpg->first; otherElem != curElem; otherElem = otherElem->next)
    {
      // If there is edge between curElem and otherElem, put it in adjMatrix.
      if (MPG_isThereEdgeBetween(curElem, otherElem))
      {
        AM_setAdjacent(otherElem->serial, curElem->serial); // PAS OP: this works because otherSerial < curSerial, always.
        ++(mpg->edges);
        if ( mpg->edges % 1000000 == 0 ) // Each 100k ledges, log a line.
          LOG("MPG PARTIAL: %dM edges.", (mpg->edges)/1000000);
      }
    }
  }
  LOG("MPG COMPLETED: %d edges.", mpg->edges);
}

bool MPG_isThereEdgeBetween(MpgElem *a, MpgElem *b)
{
  return (Atom_getBondType(a->atom[0], b->atom[0]) 
          == Atom_getBondType(a->atom[1], b->atom[1]));
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
  // Init MPG
  MPG* mpg = malloc(sizeof(MPG));
  mpg->first = NULL;
  mpg->last = NULL;
  mpg->elements = 0;
  mpg->edges = 0;
  //
  _mpgEdgesCounter = 0; //initializing the static
  LOG("Starting MPG_buildSet...");
  MPG_buildSet(mpg, g1, g2);

  LOG("Allocating AdjacencyMatrix...");
  AM_initialize(mpg->elements);

  LOG("Starting MPG_computeEdges...");
  MPG_computeEdges(mpg);
  return mpg;
}

//eof
