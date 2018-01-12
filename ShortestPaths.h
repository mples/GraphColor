#pragma once
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


