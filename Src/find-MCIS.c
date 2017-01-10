#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "Protein.h"
#include "MPG.h"
#include "ProteinParser.h"
#include "Logger.h"

/* Running CAAAAALIS! */
int main(int argc, char** argv) {
  //This must be passed 2 args, the 2 files from PDB.
  //

  // Logger *logger = Logger_newLogger();
  Logger_initializeStaticLogger();
  
  LOG("Parsing protein 1...");
  Protein* p1 = parseProteinFile(argv[1]);
  LOG("Parsing protein 2...");
  Protein* p2 = parseProteinFile(argv[2]);

  // DEBUG: printing the count of atoms in the parsed proteins
  LOG("Atoms in p1: %d", Protein_countAtoms(p1));
  LOG("Atoms in p2: %d", Protein_countAtoms(p2));
  
  // Reducing both proteins to their first connected components
  LOG("Reducing protein 1...");
  Protein_reduceToFirstConnectedComponent(p1);
  LOG("Reducing protein 2...");
  Protein_reduceToFirstConnectedComponent(p2);

  // DEBUG: printing the count of atoms in the parsed proteins
  LOG("Atoms in p1: %d", Protein_countAtoms(p1));
  LOG("Atoms in p2: %d", Protein_countAtoms(p2));
  
  //Graph_print(p1->graph);
  LOG("Building MPG...");
  MPG* mpg = MPG_buildMPG(p1, p2);

  //Now we have to find the max clique

}

//eof
