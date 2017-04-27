#include "ProteinParser.h"

/* PDB file parsing */
// What we want to do is to open a file and parse byte-per-byte,
// since lines have a fixed positions for items
void parseHELIX(Protein* protein, char* line) {
  char start[10], end[10];

  memset(start, '\0', sizeof(start));
  memset(end, '\0', sizeof(end));

  int seqNumStart, seqNumEnd;
  strncpy(start, &line[21], 4);
  strncpy(end, &line[33], 4);
  seqNumStart = (int) strtol(start, NULL, 10);
  seqNumEnd = (int) strtol(end, NULL, 10);
  Protein_addSecStructure(protein, HELIX, seqNumStart, seqNumEnd);
}

void parseSHEET(Protein* protein, char* line) {
  char start[10], end[10];

  memset(start, '\0', sizeof(start));
  memset(end, '\0', sizeof(end));

  int seqNumStart, seqNumEnd;
  strncpy(start, &line[22], 4);
  strncpy(end, &line[33], 4);
  seqNumStart = (int) strtol(start, NULL, 10);
  seqNumEnd = (int) strtol(end, NULL, 10);
  Protein_addSecStructure(protein, SHEET, seqNumStart, seqNumEnd);
}

void parseATOM(Protein* protein, char* line) {
  char elementS[10], serialS[10], amminoAcidS[10], resSeqNoS[10], xS[10], yS[10], zS[10];

  memset(elementS, '\0', sizeof(elementS));
  memset(serialS, '\0', sizeof(serialS));
  memset(amminoAcidS, '\0', sizeof(amminoAcidS));
  memset(resSeqNoS, '\0', sizeof(resSeqNoS));
  memset(xS, '\0', sizeof(xS));
  memset(yS, '\0', sizeof(yS));
  memset(zS, '\0', sizeof(zS));

  strncpy(serialS, line+6, 5);
  strncpy(resSeqNoS, line+22, 4);
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
  
  //printf("%d,%d,%d,%d,%d\n", serial, resSeqNo, x, y, z);

  //actually adding to protein's graph
  Protein_addElem(protein, element, serial, amminoAcid, resSeqNo, x, y, z);
}

Protein* parseProteinFile(char* fname) {
  char mode = 'r';
  FILE* f = fopen(fname, &mode);
  if (f == NULL)
    return NULL;
  Protein* protein = Protein_newProtein();
  char * line;
  size_t len = 256;
  ssize_t read;

  line = malloc(len * sizeof(char));
  memset(line, '\0', len);

  bool stop = FALSE;
  
  while ( !stop && (read = getline(&line, &len, f)) != -1 ) {
    if (strncmp(line, "ATOM", 4) == 0) {
      //printf(line); //debug
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
    else if (strncmp(line, "TER", 3) == 0) {
      stop = TRUE;
    }
    memset(line, '\0', len);
  }

  free(line);
  fclose(f);
  //printf("File parsing completed!\n"); //debug
  return protein;
}

//eof
