#include "Protein.h"

/* Const definition */
const float MIN_DIST_COV_BOND = 1;
const float MAX_DIST_COV_BOND = 2;
const float MAX_DIST_NCOV_BOND = 3.2;

const int NUM_ELEMENTS = 7;
const char* ELEMENTS[] = {"H", "D", "N", "C", "O", "S", "Se"};
const int NUM_AMMINO_ACIDS = 20;
const char* AMMINO_ACIDS[] = {"ALA","ARG","ASN","ASP","CYS","GLN","GLU","GLY","HIS","ILE","LEU","LYS","MET","PHE","PRO","SER","THR","TRP","TYR","VAL"};

/* Methods */

Element parseElement(char* elemS) {
  int i = 0;
  int len = 2;
  char* tmpStr;
  //printf("(1)elemS: %s\n", elemS);
  if ( *elemS == ' ' ) {
    //printf("(2)1st char of elemS should be space: \"%c\"\n", *elemS);
    ++elemS;
    len = 1;
  }
  tmpStr = malloc(len*sizeof(char));
  strncpy(tmpStr,elemS,len);
  //printf("(3)tmpStr: \"%s\"\n", tmpStr);
  while (i < NUM_ELEMENTS) {
    if (strncmp(tmpStr, ELEMENTS[i], len) == 0) {
      free(tmpStr);
      return (Element) i;
    }
    else
      ++i;
  }
  printf("FATAL: Unrecognized element %s\n", tmpStr);
  free(tmpStr);
  exit(666); //kill the process right here
}

AmminoAcid parseAmminoAcid(char* amminoS) {
  int i = 0;
  while (i < NUM_AMMINO_ACIDS) {
    if (strncmp(amminoS, AMMINO_ACIDS[i], 3) == 0)
      return (AmminoAcid) i;
    else
      ++i;
  }
  // If unrecognized, return OTHer.
  return OTH;
}

void AtomList_append(AtomListElem** this, AtomListElem* new) {
  //new must be well-formed, i.e. with new.next = NULL
  if (this == NULL)
    printf("You cannot pass this = NULL to AtomListAppend...things are gonna go bad very soon...");
  if (*(this) != NULL) {
    AtomListElem* cur = *(this);
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
  AtomListElem* newAdj = malloc(sizeof(AtomListElem));
  newAdj->atom = new;
  newAdj->next = NULL;
  //
  if (bondType == COV)
    AtomList_append( &(this->adjCov), newAdj);
  else
    AtomList_append( &(this->adjNCov), newAdj);
}

float Atom_distance(Atom* a, Atom* b) {
  return sqrt(pow(a->x - b->x, 2) + pow(a->y - b->y, 2) + pow(a->z - b->z, 2)) / 1000;
}

void Atom_clear(Atom * atom)
{
  AtomListElem * cur = atom->adjCov;
  AtomListElem * next;
  while (cur != NULL)
  {
    next = cur->next;
    free(cur);
    cur = next;
  }
  atom->adjCov = NULL;
  cur = atom->adjNCov;
  while (cur != NULL)
  {
    next = cur->next;
    free(cur);
    cur = next;
  }
  atom->adjNCov = NULL;
  free(atom);
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

BondType Atom_getBondType(Atom *a, Atom *b) {
  /* It might actually be quicker to calculate the bond each time through Atom_calcBond()... */
  AtomListElem *cur = a->adjCov;
  while (cur != NULL) {
    if (cur->atom->serial == b->serial)
      return COV;
    cur = cur->next;
  }
  cur = a->adjNCov;
  while (cur != NULL) {
    if (cur->atom->serial == b->serial)
      return NCOV;
    cur = cur->next;
  }
  return NONE;
}

SecStructure getSecondaryStructure(Protein* protein, int resSeqNum) {
  SecStructureList cur = protein->secStructures;
  while (cur != NULL) {
    if ( (resSeqNum >= cur->seqNumStart)
	&& (resSeqNum <= cur->seqNumEnd) ) {
      return cur->secStructure;
    }
    cur = cur->next;
  }
  return RCOIL;
}

Atom* Atom_newAtom() {
  Atom* newAtom = malloc(sizeof(Atom));
  newAtom->element = H;
  newAtom->serial = 0;
  newAtom->x = 0;
  newAtom->y = 0;
  newAtom->z = 0;
  newAtom->adjCov = NULL;
  newAtom->adjNCov = NULL;
  newAtom->secStructure = RCOIL;
  newAtom->visitedCC = FALSE;
  return newAtom;
}

void Graph_add(Graph* graph, Element element, int serial, AmminoAcid amminoAcid, SecStructure secondaryStructure, int x, int y, int z) {
  //Creating the new data structures
  Atom* newAtom = Atom_newAtom();
  newAtom->element = element;
  newAtom->serial = serial;
  newAtom->amminoAcid = amminoAcid;
  newAtom->x = x;
  newAtom->y = y;
  newAtom->z = z;
  newAtom->secStructure = secondaryStructure;
  //
  AtomListElem* newElem = malloc(sizeof(AtomListElem));
  newElem->atom = newAtom;
  newElem->next = NULL;
  //Now iterating on the existing graph, and checking if the
  //new friend can make any bond with anyone.
  AtomListElem* cur = *graph;
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
  printf("Printing graph...\n"); //debug
  while (graph != NULL) {
    printf("(%d) %s C:[", graph->atom->serial, ELEMENTS[graph->atom->element]);
    AtomListElem* it = graph->atom->adjCov;
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

void Graph_clear(Graph * graph)
{
  AtomListElem * cur = *graph;
  AtomListElem * next;
  while (cur != NULL)
  {
    next = cur->next;
    Atom_clear(cur->atom);
    free(cur);
    cur = next;
  }
  *graph = NULL;
}

// void Graph_getSerialsOfFirstConnectedComponent(Graph graph, IntSet *serialsptr) {
//   /* Some good old recursion... */
//   if (graph == NULL || IntSet_isValueInSet(*serialsptr, graph->atom->serial))
//     return;
//   else {
//     Atom *atomptr = graph->atom;
//     IntSet_put(serialsptr, atomptr->serial);
//     AtomListElem *adjAtom = atomptr->adjCov;
//     while (adjAtom != NULL)
//       Graph_getSerialsOfFirstConnectedComponent(adjAtom, serialsptr);
//     adjAtom = atomptr->adjNCov;
//     while (adjAtom != NULL)
//       Graph_getSerialsOfFirstConnectedComponent(adjAtom, serialsptr);
//   }
// }

void Graph_discoverFirstConnectedComponent(Graph graph) {
  /* Some good old recursion... */
  if (graph == NULL || graph->atom->visitedCC)
    return;
  else {
    Atom *atomptr = graph->atom;
    atomptr->visitedCC = TRUE;
    AtomListElem *adjAtom = atomptr->adjCov;
    while (adjAtom != NULL) {
      Graph_discoverFirstConnectedComponent(adjAtom);
      adjAtom = adjAtom->next;
    }
    adjAtom = atomptr->adjNCov;
    while (adjAtom != NULL) {
      Graph_discoverFirstConnectedComponent(adjAtom);
      adjAtom = adjAtom->next;
    }
  }
}

SecStructureListElem* SecStructureListElem_newSecStructureListElem(SecStructure secStructure, int seqNumStart, int seqNumEnd) {
  SecStructureListElem* secStructureListElem = malloc(sizeof(SecStructureListElem));
  secStructureListElem->secStructure = secStructure;
  secStructureListElem->seqNumStart = seqNumStart;
  secStructureListElem->seqNumEnd = seqNumEnd;
  secStructureListElem->next = NULL;
  return secStructureListElem;
}

void SecStructureList_add(SecStructureList* secStructureList, SecStructure secStructure, int seqNumStart, int seqNumEnd) {
  SecStructureListElem* newStruct = SecStructureListElem_newSecStructureListElem(secStructure, seqNumStart, seqNumEnd);
  SecStructureListElem* cur = *secStructureList;
  if (cur != NULL) {
    int i = 0;
    while (cur->next != NULL) {
      cur = cur->next;
      ++i;
    }
    cur->next = newStruct;
  }
  else {
    *secStructureList = newStruct;
  }
}

void SecStructureListElem_clear(SecStructureListElem * secStructureListElem)
{
  SecStructureListElem * cur = secStructureListElem;
  SecStructureListElem * next;
  while (cur != NULL)
  {
    next = cur->next;
    free(cur);
    cur = next;
  }
}

Protein* Protein_newProtein() {
  Protein* protein = malloc(sizeof(Protein));
  protein->graph = NULL;
  protein->secStructures = NULL;
  return protein;
}

void Protein_addElem(Protein* protein, Element element, int serial, AmminoAcid amminoAcid, int residueSequenceNumber, int x, int y, int z) {
  Graph_add(&(protein->graph), element, serial, amminoAcid, getSecondaryStructure(protein, residueSequenceNumber), x, y, z);
}

void Protein_addSecStructure(Protein* protein, SecStructure secStructure, int seqNumStart, int seqNumEnd) {
  SecStructureList_add(&(protein->secStructures), secStructure, seqNumStart, seqNumEnd);
}

void Protein_clear(Protein * protein)
{
  Graph_clear(&(protein->graph));
  SecStructureListElem_clear(protein->secStructures);
  free(protein);
}

/* Protein_countAtoms
* is to count atoms in the protein's graph.
*/
int Graph_countAtoms(Graph graph)
{
  int counter = 0;
  while (graph != NULL) {
    ++counter;
    graph = graph->next;
  }
  return counter;
}

int Protein_countAtoms(Protein *protein)
{
  return Graph_countAtoms(protein->graph);
  
}

// void Protein_reduceToFirstConnectedComponent(Protein *protein) {
//   IntSet serials = IntSet_new();
//   Graph_getSerialsOfFirstConnectedComponent(protein->graph, &serials);
//   AtomListElem *cur = protein->graph;
//   AtomListElem *tmp = NULL; //this is just used when removing an element
//   while (cur != NULL && !IntSet_isValueInSet(serials, cur->atom->serial)) {
//     protein->graph = cur->next;
//     free(cur->atom);
//     free(cur);
//     cur = protein->graph;
//   }
//   if (cur == NULL) 
//     return;
//   while (cur->next != NULL && !IntSet_isValueInSet(serials, cur->next->atom->serial)) {
//     tmp = cur->next;
//     cur->next = cur->next->next;
//     free(tmp->atom);
//     free(tmp);
//   }
//   IntSet_free(serials);
// }

void Protein_reduceToFirstConnectedComponent(Protein *protein) {
  Graph_discoverFirstConnectedComponent(protein->graph);
  AtomListElem *cur = protein->graph;
  AtomListElem *tmp = NULL; //this is just used when removing an element
  while (cur != NULL && !cur->atom->visitedCC) {
    protein->graph = cur->next;
    free(cur->atom);
    free(cur);
    cur = protein->graph;
  }
  if (cur == NULL) 
    return;
  while (cur->next != NULL && !cur->next->atom->visitedCC) {
    tmp = cur->next;
    cur->next = cur->next->next;
    free(tmp->atom);
    free(tmp);
  }
}

// IntSet IntSet_new() {
//   IntSet is = malloc(sizeof(IntSetElem));
//   is->value = 0;
//   is->next = NULL;
//   return is;
// }
  // IntSet IntSet_new(int cardinality) {
  //   return calloc((sizeof(long)-1+cardinality)/sizeof(long), sizeof(long)); //rounding size up and calloc used for 0-ed mem
  // }

// void IntSet_put(IntSet *setptr, int newval) {
//   if (setptr == NULL) {
//     printf("FATAL: Called IntSet_append on a NULL pointer!\n");
//     return;
//   }

//   IntSet newelem = malloc(sizeof(IntSetElem));
//   newelem->value = newval;
//   newelem->next = NULL;
  
//   if (*setptr == NULL) {
//     *setptr = newelem;
//   }
//   else {
//     IntSet cur = *setptr;
//     while (cur->next != NULL && cur->next->value < newval)
//       cur = cur->next;
//     if (cur->value == newval)
//       return; //don't add duplicates
//     /* insert the newelem in the set */
//     newelem->next = cur->next;
//     cur->next = newelem;
//   }
// }
// void IntSet_put(IntSet *setptr, int newval) {
//   //todo
// }

// void IntSet_free(IntSet set) {
//   IntSet next = NULL;
//   while (set != NULL) {
//     next = set->next;
//     free(set);
//     set = next;
//   }
// }

// bool IntSet_isValueInSet(IntSet set, int val) {
//   while(set != NULL) {
//     if (set->value == val)
//       return TRUE;
//     set = set->next;
//   }
//   return FALSE;
// }

//eof
