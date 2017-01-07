#ifndef MPG_H_
#define MPG_H_

#include "Protein.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

/* Modular product group definition */
typedef struct AdjMpgElem {
     struct MpgElem* mpgElem;
     struct AdjMpgElem* next;
} AdjMpgElem;

typedef struct MpgElem {
  Atom* atom[2]; //ptrs to the atoms from the two underlying proteins
  AdjMpgElem* adj;
  struct MpgElem* next;
} MpgElem;

typedef MpgElem* MPG;

/* Functions declarations */
MpgElem* MPG_getNewElem(Atom* a0, Atom* a1);
bool MPG_areAtomsCompatible(Atom* a, Atom* b);
void MPG_buildSet(MPG *mpg, Graph g1, Graph g2);
void MPG_computeEdges(MPG *mpg);
MPG* MPG_buildMPG(Protein* p1, Protein* p2);

#endif // MPG_H_
