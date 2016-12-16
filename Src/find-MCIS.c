#include<stdio.h>
#include<stdlib.h>
#include<math.h>

const float MIN_DIST_COV_BOND = 1;
const float MAX_DIST_COV_BOND = 2;
const float MAX_DIST_NCOV_BOND = 3.2;

const char* ELEMENTS[] = {"H", "D", "N", "C", "O", "S", "Se"};
typedef enum Element { H, D, N, C, O, S, Se } Element;
typedef enum BondType { NONE, COV, NCOV } BondType;

typedef struct Atom {
  Element element; //facciamo un enum di fatto (potremmo anche usare un bit-field per ridurre lo spazio)
  int serial; //forse utile per fare matching con le strutture HELIX/SHEET
  int x; //position in x axis (int because we represent in mA° (milli Angstrom))
  int y;
  int z;
  struct Elem* adjCov;
  struct Elem* adjNCov;
} Atom;

typedef struct Elem {
  Atom* atom;
  struct Elem* next;
} Elem;

typedef Elem* ElemList;

void ElemList_append(Elem** this, Elem* new) {
  //new must be well-formed, i.e. with new.next = NULL
  if ( this == NULL)
    printf("You cannot pass this = NULL to ElemListAppend...thing are gonna go bad very soon...");
  if (*(this) != NULL) {
    Elem* cur = *(this);
    while(cur->next != NULL) {
      cur = cur->next;
    }
    cur->next = new;
  }
  else {
    *(this) = new;
  }
}

void Atom_addAdjElem(Atom* this, Atom* new, BondType bondType) {
  Elem* newAdj = malloc(sizeof(Elem));
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

typedef struct Elem* Graph;

void Graph_add(Graph* graph, Element element, int serial, int x, int y, int z) {
  Atom* newAtom = malloc(sizeof(Atom));
  newAtom->element = element;
  newAtom->serial = serial;
  newAtom->x = x;
  newAtom->y = y;
  newAtom->z = z;
  newAtom->adjCov = NULL;
  newAtom->adjNCov = NULL;
  Elem* newElem = malloc(sizeof(Elem));
  newElem->atom = newAtom;
  newElem->next = NULL;
  //
  Elem* cur = *graph;
  if(cur != NULL) {
    while(cur->next != NULL) {
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

void Graph_print(Graph graph) {
  while (graph != NULL) {
    printf("(%d) %s C:[", graph->atom->serial, ELEMENTS[graph->atom->element]);
    Elem* it = graph->atom->adjCov;
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


int main(int argc, char** argv) {
  //This must be passed 2 args, the 2 files from PDB.
  // H-C-N, C-S-H
  //
  Graph g = NULL;
  for (int i=0; i<10000; i+=6) {
    Graph_add(&g, H, i+0, 0,0,10000*i); //remember, coordinates are in mA°
    Graph_add(&g, C, i+1, 0,1700,10000*i);
    Graph_add(&g, N, i+2, 0,3400,10000*i);
    Graph_add(&g, C, i+3, 6000,0,10000*i);
    Graph_add(&g, S, i+4, 6000,1500,10000*i);
    Graph_add(&g, H, i+5, 6000,3000,10000*i);
  }

  Graph_print(g);
}

//eof
