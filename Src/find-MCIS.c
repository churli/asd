#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

/* CONST+ENUM declarations */
const float MIN_DIST_COV_BOND = 1;
const float MAX_DIST_COV_BOND = 2;
const float MAX_DIST_NCOV_BOND = 3.2;

const int NUM_ELEMENTS = 7;
const char* ELEMENTS[] = {"H", "D", "N", "C", "O", "S", "Se"};
typedef enum Element { H, D, N, C, O, S, Se } Element;
typedef enum BondType { NONE, COV, NCOV } BondType;
typedef enum SecStructure { RCOIL, HELIX, SHEET } SecStructure;

/*
STRUCTURES declarations
*/
typedef struct Atom {
  Element element; //facciamo un enum di fatto (potremmo anche usare un bit-field per ridurre lo spazio)
  int serial; //forse non serve proprio a un cazzo
  int residualSeqNum; //questo serve per matchare le secondary structures
  int x; //position in x axis (int because we represent in mA° (milli Angstrom))
  int y;
  int z;
  struct lElem* adjCov;
  struct lElem* adjNCov;
  SecStructure secStructure;
} Atom;

typedef struct lElem {
  Atom* atom;
  struct lElem* next;
} lElem;

typedef lElem* ElemList;

typedef struct SecStructureListElem {
  //Here we report each secodnary structure with its begin/end residual
  //sequence numbers and the type, HELIX/SHEET, of the structure.
  int seqNumStart;
  int seqNumEnd;
  SecStructure secStructure;
  struct SecStructureListElem* next;
} SecStructureListElem;

typedef struct SecStructureListElem* SecStructureList;

typedef struct lElem* Graph;

/* Protein structure definition */
typedef struct Protein {
  Graph graph;
  SecStructureList secStructures;
} Protein;

/* Modular product group definition */
typedef struct AdjMpgElem {
     struct MpgElem* mpgElem;
     struct AdjMpgElem* next;
} AdjMpgElem;

typedef struct MpgElem {
  Atom* atom[2]; //ptrs to the atoms from the two underlying proteins
  AdjMpgElem* adj;
} MpgElem;

typedef struct MPG {
  MpgElem* first;
} MPG;

/*
Function+methods declarations
*/
Element parseElement(char* elemS) {
  int i = 0;
  /* char* elemLoc; */
  printf("%s\n",elemS);
  if ( strcmp(elemS," ") == 0 )
    ++elemS;
  /* if ( strcmp(elemS," ") == 0 ) { */
  /*   elemLoc = malloc(sizeof(char)); */
  /*   *elemLoc = *(elemS+1); */
  /* } */
  /* else { */
  /*   elemLoc = malloc(2*sizeof(char)); */
  /*   strcpy(elemLoc, elemS); */
  /* } */
  printf("%s\n",elemS);
  while (i < NUM_ELEMENTS) {
   if (strcmp(elemS,ELEMENTS[i]) == 0)
     return (Element) i;
   else
     ++i;
  }
  printf("FATAL: Unrecognized element %s\n", elemS);
  exit(666); //kill the process right here
}

void ElemList_append(lElem** this, lElem* new) {
  //new must be well-formed, i.e. with new.next = NULL
  if ( this == NULL)
    printf("You cannot pass this = NULL to ElemListAppend...thing are gonna go bad very soon...");
  if (*(this) != NULL) {
    lElem* cur = *(this);
    while(cur->next != NULL) {
      cur = cur->next;
    }
    cur->next = new;
  }
  else {
    *(this) = new;
  }
}
//

//
void Atom_addAdjElem(Atom* this, Atom* new, BondType bondType) {
  lElem* newAdj = malloc(sizeof(lElem));
  newAdj->atom = new;
  newAdj->next = NULL;
  //
  if (bondType == COV)
    ElemList_append( &(this->adjCov), newAdj);
  else
    ElemList_append( &(this->adjNCov), newAdj);
}

float Atom_distance(Atom* a, Atom* b) {
  return sqrt(pow(a->x - b->x, 2) + pow(a->y - b->y, 2) + pow(a->z - b->z, 2)) / 1000;
}

BondType Atom_calcBond(Atom* a, Atom* b) {
  float dist = Atom_distance(a,b);
  if (dist < MIN_DIST_COV_BOND)
    return NONE;
  else if (dist <= MAX_DIST_COV_BOND)
    return COV;
  else if (dist <= MAX_DIST_NCOV_BOND)
    return NCOV;
  else
    return NONE;
}

void Atom_checkBond(Atom* a, Atom* b) {
  BondType bondType = Atom_calcBond(a,b);
  if (bondType != NONE) {
    Atom_addAdjElem(a,b,bondType);
    Atom_addAdjElem(b,a,bondType);
  }
}

SecStructure getSecondaryStructure(Protein protein, int resSeqNum) {
  SecStructureList cur = protein.secStructures;
  while (cur != NULL) {
    if ( (resSeqNum >= cur->seqNumStart)
	&& (resSeqNum <= cur->seqNumEnd) ) {
      return cur->secStructure;
    }
    cur = cur->next;
  }
  return RCOIL;
}

void Graph_add(Graph* graph, Element element, int serial, SecStructure secondaryStructure, int x, int y, int z) {
  //Creating the new data structures
  Atom* newAtom = malloc(sizeof(Atom));
  newAtom->element = element;
  newAtom->serial = serial;
  newAtom->x = x;
  newAtom->y = y;
  newAtom->z = z;
  newAtom->adjCov = NULL;
  newAtom->adjNCov = NULL;
  //This has to be properly set, either here or somewhere else, using data
  //from the SHEET/HELIX parameters.
  newAtom->secStructure = secondaryStructure;
  //
  lElem* newElem = malloc(sizeof(lElem));
  newElem->atom = newAtom;
  newElem->next = NULL;
  //Now iterating on the existing graph, and checking if the
  //new friend can make any bond with anyone.
  lElem* cur = *graph;
  if (cur != NULL) {
    while (cur->next != NULL) {
      Atom_checkBond(cur->atom, newAtom);
      cur = cur->next;
    }
    Atom_checkBond(cur->atom, newAtom);
    cur->next = newElem;
  }
  else {
    *graph = newElem;
  }
}

// Printing
void Graph_print(Graph graph) {
  while (graph != NULL) {
    printf("(%d) %s C:[", graph->atom->serial, ELEMENTS[graph->atom->element]);
    lElem* it = graph->atom->adjCov;
    while (it != NULL) {
      if (it != graph->atom->adjCov) //if this is not the first cycle
	printf(","); //put a comma separator
      printf("(%d:%s)", it->atom->serial, ELEMENTS[it->atom->element]);
      it = it->next;
    }
    printf("] NC:[");
    it = graph->atom->adjNCov;
    while (it != NULL) {
      if (it != graph->atom->adjNCov) //if this is not the first cycle
	printf(","); //put a comma separator
      printf("(%d:%s)", it->atom->serial, ELEMENTS[it->atom->element]);
      it = it->next;
    }
    printf("]\n");
    graph = graph->next;
  }
}

SecStructureListElem* SecStructureListElem_newSecStructureListElem(SecStructure secStructure, int seqNumStart, int seqNumEnd) {
  SecStructureListElem* secStructureListElem = malloc(sizeof(SecStructureListElem));
  secStructureListElem->secStructure = secStructure;
  secStructureListElem->seqNumStart = seqNumStart;
  secStructureListElem->seqNumEnd = seqNumEnd;
  return secStructureListElem;
}

void SecStructureList_add(SecStructureList* secStructureList, SecStructure secStructure, int seqNumStart, int seqNumEnd) {
  SecStructureListElem* newStruct = SecStructureListElem_newSecStructureListElem(secStructure, seqNumStart, seqNumEnd);
  SecStructureListElem* cur = *secStructureList;
  if (cur != NULL) {
    while (cur->next != NULL) {
      cur = cur->next;
    }
    cur->next = newStruct;
  }
  else {
    *secStructureList = newStruct;
  }
}

Protein* Protein_newProtein() {
  Protein* protein = malloc(sizeof(Protein));
  protein->graph = NULL;
  protein->secStructures = NULL;
}

void Protein_addElem(Protein protein, Element element, int serial, int residueSequenceNumber, int x, int y, int z) {
  Graph_add(&(protein.graph), element, serial, getSecondaryStructure(protein, residueSequenceNumber), x, y, z);
}

void Protein_addSecStructure(Protein protein, SecStructure secStructure, int seqNumStart, int seqNumEnd) {
  SecStructureList_add(&(protein.secStructures), secStructure, seqNumStart, seqNumEnd);
}

/* PDB file parsing */
// What we want to do is to open a file and parse byte-per-byte,
// since lines have a fixed positions for items
void parseHELIX(Protein protein, char* line) {
  char start[4], end[4];
  int seqNumStart, seqNumEnd;
  strncpy(start, &line[21], 4);
  strncpy(end, &line[33], 4);
  seqNumStart = (int) strtol(start, NULL, 10);
  seqNumEnd = (int) strtol(end, NULL, 10);
  Protein_addSecStructure(protein, HELIX, seqNumStart, seqNumEnd);
}

void parseSHEET(Protein protein, char* line) {
  char start[4], end[4];
  int seqNumStart, seqNumEnd;
  strncpy(start, &line[22], 4);
  strncpy(end, &line[33], 4);
  seqNumStart = (int) strtol(start, NULL, 10);
  seqNumEnd = (int) strtol(end, NULL, 10);
  Protein_addSecStructure(protein, SHEET, seqNumStart, seqNumEnd);
}

void parseATOM(Protein protein, char* line) {
  char elementS[2], serialS[5], resSeqNoS[4], xS[8], yS[8], zS[8];
  strncpy(serialS, &line[6], 5);
  strncpy(resSeqNoS, &line[22], 4);
  strncpy(xS, &line[30], 8);
  strncpy(yS, &line[38], 8);
  strncpy(zS, &line[46], 8);
  strncpy(elementS, &line[76], 2);
  printf("%s,%s,%s,%s,%s,%s\n",elementS, serialS, resSeqNoS, xS, yS, zS);//debug
  Element element = parseElement((char*)elementS);
  int serial = (int) strtol(serialS, NULL, 10);
  int resSeqNo = (int) strtol(resSeqNoS, NULL, 10);
  int x = (int) (strtof(xS, NULL) * 1000);
  int y = (int) (strtof(yS, NULL) * 1000);
  int z = (int) (strtof(zS, NULL) * 1000);
}

Protein* parseProteinFile(char* fname) {
  char mode = 'r';
  FILE* f = fopen(fname, &mode);
  if (f == NULL)
    return NULL;
  Protein* protein = Protein_newProtein();
  char* line = NULL;
  size_t len = 0;
  ssize_t read;
  
  while ( (read = getline(&line, &len, f)) != -1 ) {
    if (strncmp(line, "ATOM", 4) == 0) {
      /* printf("len = %d\tread = %d\n", len, read); */
      printf(line);
      parseATOM(*protein, line);
    }
    else if (strncmp(line, "SHEET", 5) == 0) {
      printf(line);
      parseSHEET(*protein, line);
    }
    else if (strncmp(line, "HELIX", 5) == 0) {
      printf(line);
      parseHELIX(*protein, line);
    }
  }

  free(line);
  fclose(f);
}

/* Running CAAAAALIS! */
int main(int argc, char** argv) {
  //This must be passed 2 args, the 2 files from PDB.
  // H-C-N, C-S-H
  //
  
  //Graph g = NULL;
  /* Graph g1 = NULL; */
  /* Graph g2 = NULL; */
  
  /* for (int i=0; i<10000; i+=6) { */
  /*   Graph_add(&g, H, i+0, 0,0,10000*i); //remember, coordinates are in mA° */
  /*   Graph_add(&g, C, i+1, 0,1700,10000*i); */
  /*   Graph_add(&g, N, i+2, 0,3400,10000*i); */
  /*   Graph_add(&g, C, i+3, 6000,0,10000*i); */
  /*   Graph_add(&g, S, i+4, 6000,1500,10000*i); */
  /*   Graph_add(&g, H, i+5, 6000,3000,10000*i); */
  /* } */
  
  /* Graph_add(&g1, H, 0, HELIX, 0,0,10000); */
  /* Graph_add(&g1, C, 1, 1, 0,1700,10000); */
  /* Graph_add(&g1, N, 2, 1, 0,3400,10000); */
  /* Graph_add(&g1, C, 3, 2, 6000,0,10000); */
  /* Graph_add(&g1, S, 4, 2, 6000,1500,10000); */
  /* Graph_add(&g1, H, 5, 2, 6000,3000,10000); */
  
  /* Graph_add(&g2, H, 0, 1, 0,0,10000); */
  /* Graph_add(&g2, C, 1, 1, 0,1700,10000); */
  /* Graph_add(&g2, N, 2, 1, 0,3400,10000); */
  /* Graph_add(&g2, H, 3, 2, 6000,0,10000); */
  /* Graph_add(&g2, C, 4, 2, 6000,1500,10000); */
  /* Graph_add(&g2, N, 5, 2, 6000,3000,10000); */

  Protein* p1 = parseProteinFile(argv[1]);
  
  Graph_print(p1->graph);
}

//eof
