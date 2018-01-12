//
// Created by mateusz on 30.12.17.
//

#ifndef GRAPHGEN_GRAPH_H
#define GRAPHGEN_GRAPH_H

#include <fstream>
#include "AdjMatrix.h"

class Graph {

public:
	AdjMatrix * AdjMat;
    Graph();
    ~Graph();
    Graph(const char* fname);
    int getV();
    int getE();
    AdjMatrix* getMatrix();
    bool loadFile(const char* fname);
protected:

private:
    int V; // vertexes count
    int E; // edged count
    //AdjMatrix* AdjMat;
    //bool loadFile(const char* fname);

};


#endif //GRAPHGEN_GRAPH_H
