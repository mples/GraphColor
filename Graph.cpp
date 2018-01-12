//
// Created by mateusz on 30.12.17.
//

#include <fstream>
#include "Graph.h"

Graph::Graph() {

}

Graph::Graph(const char* fname) {
    //adjMat = new AdjMatrix() ;
    loadFile(fname);
}

Graph::~Graph() {

    delete AdjMat;

}

bool Graph::loadFile(const char* fname) {

    std::ifstream file;
    file.open(fname);
    if( !file.good() ){
        return false;
    }

    file >> V;
    file >> E;

    AdjMat = new AdjMatrix(V,0);

    int r,c,v;

    for (int i = 0 ; i < E ; ++i){
        file >> r >> c >> v;
        AdjMat->setValue(r,c,v);
    }
    file.close();
    return true;

}

int Graph::getV() {
    return V;
}

int Graph::getE() {
    return E;
}

AdjMatrix *Graph::getMatrix() {
    return AdjMat;
}
