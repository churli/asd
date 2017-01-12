#ifndef MPG_H_
#define MPG_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "Common.h"
#include "Protein.h"
#include "Logger.h"

/* Modular product group definition */
typedef struct AdjMpgElem {
     struct MpgElem* mpgElem;
     struct AdjMpgElem* next;
} AdjMpgElem;

typedef AdjMpgElem* AdjMpgElemList;

typedef struct MpgElem {
  Atom* atom[2]; //ptrs to the atoms from the two underlying proteins
  AdjMpgElem* adj;
  struct MpgElem* next;
} MpgElem;

typedef MpgElem* MPG;

/* Functions declarations */
void AdjMpgElemList_append(AdjMpgElemList this, AdjMpgElem *new);
void MpgElem_addAdjElem(MpgElem *this, MpgElem *new);

MpgElem* MPG_getNewElem(Atom* a0, Atom* a1);
bool MPG_areAtomsCompatible(Atom* a, Atom* b);
void MPG_buildSet(MPG *mpg, Graph g1, Graph g2);
// void MPG_computeEdges(MPG *mpg);
void MPG_addElementAndComputeEdges(MPG *mpg, MpgElem *newElem);
MPG* MPG_buildMPG(Protein* p1, Protein* p2);

#endif // MPG_H_
