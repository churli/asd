#Makefile for find-MCIS (Maximal Common Induced Subgraph)

LIBRARIES=Common.h Protein.c MPG.c ProteinParser.c Logger.c AdjacencyMatrix.c BronKerbosch.c Tomita.c Sets.c VBronKerbosch.c VectorSet.c

#GCC_OPTIONS=-g -Wall -Wpedantic
GCC_OPTIONS=-g

compile:
	gcc -o ../Target/find-MCIS find-MCIS.c ${LIBRARIES} -lm ${GCC_OPTIONS}

deploy:
	rsync -hPv ../Target/find-MCIS tommasovm:FindMCIS/.

#EOF
