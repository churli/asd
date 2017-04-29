#ifndef MPG_H_
#define MPG_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "Common.h"
#include "Protein.h"
#include "AdjacencyMatrix.h"
#include "Sets.h"
#include "Logger.h"

/* Modular product group definition */
// typedef struct AdjMpgElem {
//      struct MpgElem* mpgElem;
//      struct AdjMpgElem* next;
// } AdjMpgElem;

// typedef AdjMpgElem* AdjMpgElemList;

typedef struct MpgElem {
  Atom* atom[2]; //ptrs to the atoms from the two underlying proteins
  //AdjMpgElem* adj;
  Set * adjacentSet;
  long serial;
  struct MpgElem* next;
} MpgElem;

typedef struct MPG {
  MpgElem* first;
  MpgElem* last;
  long elements;
  long edges;
} MPG;

// Statics
//static unsigned long _mpgEdgesCounter; //debug //apparently long isn't long enough at all...we easily get overflows here...
static bool _avoidAmminoCheck;

/* Functions declarations */
// void AdjMpgElemList_append(AdjMpgElemList this, AdjMpgElem *new);
// void MpgElem_addAdjElem(MpgElem *this, MpgElem *new);

MpgElem* MPG_getNewElem(Atom* a0, Atom* a1, long serial);
bool MPG_areAtomsCompatible(Atom* a, Atom* b);
void MPG_clear(MPG * mpg);
void MPG_buildSet(MPG *mpg, Graph g1, Graph g2);
void MPG_addElement(MPG *mpg, MpgElem *newElem);
void MPG_computeEdges(MPG *mpg);
bool MPG_isThereEdgeBetween(MpgElem *a, MpgElem *b);
MPG* MPG_buildMPG(Protein* p1, Protein* p2, bool filterByAmminoCheck);

#endif // MPG_H_
