#include "ShortestPaths.h"



ShortestPaths::ShortestPaths(Graph *gr)
{
	graphMatrix = gr->getMatrix();
	pathsMatix = new AdjMatrix(graphMatrix->getSize(), UINT_MAX);
}


ShortestPaths::~ShortestPaths()
{
}

void ShortestPaths::calculatePaths()
{
	int size = pathsMatix->getSize();
	for (int i = 0; i < size; ++i) {
		pathsMatix->setValue(i, i, 0);
	}
	
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < i + 1; ++j) {
			if(graphMatrix->getValue(i,j) != 0)
				pathsMatix->setValue(i,j,graphMatrix->getValue(i,j) );
		}
	}

	for (int k = 0; k < size; ++k) {
		for (int i = 0; i < size; ++i) {
			for (int j = 0; j < size; ++j) {
				if (pathsMatix->getValue(i, k) == UINT_MAX || pathsMatix->getValue(k, j) == UINT_MAX)
					continue;
				if (pathsMatix->getValue(i, j) > pathsMatix->getValue(i, k) + pathsMatix->getValue(k, j)) {
					pathsMatix->setValue(i, j, pathsMatix->getValue(i, k) + pathsMatix->getValue(k, j));
				}
			}
		}
	}

}

AdjMatrix * ShortestPaths::getShrtsPaths()
{
	return pathsMatix;
}


