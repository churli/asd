#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "Protein.h"
#include "MPG.h"
#include "ProteinParser.h"

/* Running CAAAAALIS! */
int main(int argc, char** argv) {
  //This must be passed 2 args, the 2 files from PDB.
  //
  
  Protein* p1 = parseProteinFile(argv[1]);
  Protein* p2 = parseProteinFile(argv[2]);

  // DEBUG: printing the count of atoms in the parsed proteins
  printf("Atoms in p1: %d\n", Protein_countAtoms(p1));
  printf("Atoms in p2: %d\n", Protein_countAtoms(p2));

  // Reducing both proteins to their first connected components
  Protein_reduceToFirstConnectedComponent(p1);
  Protein_reduceToFirstConnectedComponent(p2);

  // DEBUG: printing the count of atoms in the parsed proteins
  printf("Atoms in p1: %d\n", Protein_countAtoms(p1));
  printf("Atoms in p2: %d\n", Protein_countAtoms(p2));

  //Graph_print(p1->graph);
  MPG* mpg = MPG_buildMPG(p1, p2);

  //Now we have to find the max clique

}

//eof
