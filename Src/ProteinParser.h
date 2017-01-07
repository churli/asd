#ifndef PROTEIN_PARSER_H_
#define PROTEIN_PARSER_H_

#include "Protein.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

void parseHELIX(Protein* protein, char* line);
void parseSHEET(Protein* protein, char* line);
void parseATOM(Protein* protein, char* line);
Protein* parseProteinFile(char* fname);

#endif // PROTEIN_PARSER_H_
