#include "ProteinParser.h"

/* PDB file parsing */
// What we want to do is to open a file and parse byte-per-byte,
// since lines have a fixed positions for items
void parseHELIX(Protein* protein, char* line) {
  char start[4] = {0, 0, 0, 0}, end[4] = {0, 0, 0, 0};
  int seqNumStart, seqNumEnd;
  strncpy(start, &line[21], 4);
  strncpy(end, &line[33], 4);
  seqNumStart = (int) strtol(start, NULL, 10);
  seqNumEnd = (int) strtol(end, NULL, 10);
  Protein_addSecStructure(protein, HELIX, seqNumStart, seqNumEnd);
}

void parseSHEET(Protein* protein, char* line) {
  char start[4], end[4];
  int seqNumStart, seqNumEnd;
  strncpy(start, &line[22], 4);
  strncpy(end, &line[33], 4);
  seqNumStart = (int) strtol(start, NULL, 10);
  seqNumEnd = (int) strtol(end, NULL, 10);
  Protein_addSecStructure(protein, SHEET, seqNumStart, seqNumEnd);
}

void parseATOM(Protein* protein, char* line) {
  char elementS[2], serialS[5], amminoAcidS[3], resSeqNoS[4], yS[8], zS[8];
  char* xS = malloc(8*sizeof(char));
  strncpy(serialS, &line[6], 5);
  strncpy(resSeqNoS, &line[22], 4);
  strncpy(amminoAcidS, &line[17], 3);
  strncpy(xS, &line[30], 8);
  strncpy(yS, &line[38], 8);
  strncpy(zS, &line[46], 8);
  strncpy(elementS, &line[76], 2);
  //printf("%s,%s,%s,%s,%s,%s\n",elementS, serialS, resSeqNoS, xS, yS, zS);//debug
  Element element = parseElement((char*)elementS);
  AmminoAcid amminoAcid = parseAmminoAcid((char*)amminoAcidS);
  int serial = (int) strtol(serialS, NULL, 10);
  int resSeqNo = (int) strtol(resSeqNoS, NULL, 10);
  int x = (int) (strtof(xS, NULL) * 1000);
  int y = (int) (strtof(yS, NULL) * 1000);
  int z = (int) (strtof(zS, NULL) * 1000);
  free(xS);
  //actually adding to protein's graph
  Protein_addElem(protein, element, serial, amminoAcid, resSeqNo, x, y, z);
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
      //printf(line);
      parseATOM(protein, line);
    }
    else if (strncmp(line, "SHEET", 5) == 0) {
      //printf(line);
      parseSHEET(protein, line);
    }
    else if (strncmp(line, "HELIX", 5) == 0) {
      //printf(line);
      parseHELIX(protein, line);
    }
  }

  free(line);
  fclose(f);
  //printf("File parsing completed!\n"); //debug
  return protein;
}

//eof
