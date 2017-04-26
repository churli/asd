#ifndef PROTEIN_H_
#define PROTEIN_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "Common.h"

/* CONST+ENUM declarations */
const float MIN_DIST_COV_BOND;
const float MAX_DIST_COV_BOND;
const float MAX_DIST_NCOV_BOND;

const int NUM_ELEMENTS;
extern const char* ELEMENTS[];
typedef enum Element { H, D, N, C, O, S, Se } Element;
typedef enum BondType { NONE, COV, NCOV } BondType;
typedef enum SecStructure { RCOIL, HELIX, SHEET } SecStructure;
typedef enum AmminoAcid { ALA, ARG, ASN, ASP, CYS, GLN, GLU, GLY, HIS, ILE, LEU, LYS, MET, PHE, PRO, SER, THR, TRP, TYR, VAL, OTH } AmminoAcid;


/*
STRUCTURES declarations
*/
typedef struct Atom {
  Element element; //facciamo un enum di fatto (potremmo anche usare un bit-field per ridurre lo spazio)
  int serial; //serve per identificarli, alla fine puo' fare comodo.
  int residualSeqNum; //questo serve per matchare le secondary structures
  int x; //position in x axis (int because we represent in mA° (milli Angstrom))
  int y;
  int z;
  struct AtomListElem* adjCov;
  struct AtomListElem* adjNCov;
  SecStructure secStructure;
  AmminoAcid amminoAcid;
  bool visitedCC; //this is used when reducing the protein to its 1st connected component.
} Atom;

typedef struct AtomListElem {
  Atom* atom;
  struct AtomListElem* next;
} AtomListElem;

typedef AtomListElem* AtomList;

typedef struct SecStructureListElem {
  //Here we report each secodnary structure with its begin/end residual
  //sequence numbers and the type, HELIX/SHEET, of the structure.
  int seqNumStart;
  int seqNumEnd;
  SecStructure secStructure;
  struct SecStructureListElem* next;
} SecStructureListElem;

typedef struct SecStructureListElem* SecStructureList;

typedef struct AtomListElem* Graph;

/* Protein structure definition */
typedef struct Protein {
  Graph graph;
  SecStructureList secStructures;
  int atomsCount;
} Protein;

/* Serial numbers int (ordered) set */
/*typedef struct IntSetElem {
  int value;S
  struct IntSetElem *next;
} IntSetElem;

typedef IntSetElem* IntSet;*/
// typedef unsigned long * IntSet; // we try with bitfields. Basically if i-th bit is 1 it means that that serial is in set

/*
Function+methods declarations
*/
// Parse string element to enum value
Element parseElement(char *elemS);
AmminoAcid parseAmminoAcid(char* amminoS);
// Methods to add elements to structs
void AtomList_append(AtomListElem** this, AtomListElem* new);
void Atom_addAdjElem(Atom* this, Atom* new, BondType bondType);
void Atom_clear(Atom * atom);
// Calculate or check things
float Atom_distance(Atom* a, Atom* b);
BondType Atom_calcBond(Atom* a, Atom* b);
void Atom_checkBond(Atom* a, Atom* b);
BondType Atom_getBondType(Atom *a, Atom *b); //Check if any 2 given atoms are adjacents to each-other
SecStructure getSecondaryStructure(Protein* protein, int resSeqNum);
// Graph
void Graph_add(Graph* graph, Element element, int serial, AmminoAcid amminoAcid, SecStructure secondaryStructure, int x, int y, int z);
void Graph_print(Graph graph);
void Graph_clear(Graph * graph);
//void Graph_getSerialsOfFirstConnectedComponent(Graph graph, IntSet *serialsptr);
void Graph_discoverFirstConnectedComponent(Graph graph);
int Graph_countAtoms(Graph graph);
// SecStructureList methods
SecStructureListElem* SecStructureListElem_newSecStructureListElem(SecStructure secStructure, int seqNumStart, int seqNumEnd);
void SecStructureList_add(SecStructureList* secStructureList, SecStructure secStructure, int seqNumStart, int seqNumEnd);
void SecStructureListElem_clear(SecStructureListElem * secStructureListElem);
// Protein methods
Protein* Protein_newProtein();
void Protein_addElem(Protein* protein, Element element, int serial, AmminoAcid amminoAcid, int residueSequenceNumber, int x, int y, int z);
void Protein_addSecStructure(Protein* protein, SecStructure secStructure, int seqNumStart, int seqNumEnd);
void Protein_clear(Protein * protein);
int Protein_countAtoms(Protein *protein);
void Protein_reduceToFirstConnectedComponent(Protein *protein); //this is used to get only the 1st connected component of a given protein
// // IntSet methods
// IntSet IntSet_new(int cardinality);
// void IntSet_put(IntSet *setptr, int newval);
// void IntSet_free(IntSet set);
// bool IntSet_isValueInSet(IntSet set, int val);

#endif // PROTEIN_H_
