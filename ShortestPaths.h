#ifndef GRAPHGEN_SHORTESTPATHS_H
#define GRAPHGEN_SHORTESTPATHS_H
#include "Graph.h"
#include <stdint.h>

class ShortestPaths
{
public:
	ShortestPaths(Graph *gr);
	~ShortestPaths();
	void calculatePaths();
	AdjMatrix* getShrtsPaths();
private:
	AdjMatrix * pathsMatix;
	AdjMatrix * graphMatrix;

};

#endif
