#include "Algoritms.h"



Algoritms::Algoritms(std::string fname, unsigned int cNum)
{
	gr = new Graph(fname);
	sp = new ShortestPaths(gr);
	sp->calculatePaths();
	colourNumber = cNum;
}


Algoritms::~Algoritms()
{
	delete gr;
	delete sp;
}
std::vector<unsigned int> Algoritms::optimalAlgoritm()
{
	unsigned int size = gr->getV();
	std::vector <unsigned int> coloringTable(size, 0);  //Tablbe for all vertices, if coloringTable i = j, that means i-vertex is painted in j-color
	unsigned int sPath = 1;
	std::vector<unsigned int > retColoring;
	do { 
		unsigned int pShPath = UINT_MAX; // minimal shortest path in permutation (variation)
		std::vector<std::vector<unsigned int>> coloredVer(colourNumber, std::vector<unsigned int>());
		for (int i = 0; i < size; ++i) {
			coloredVer[coloringTable[i]].push_back(i);
		}
		for (int i = 0; i < colourNumber; ++i) {
			if (coloredVer[i].empty() || coloredVer[i].size() == 1) {
				continue;
			}

			std::vector<unsigned int> combinationVector(coloredVer[i].size(), 0);
			combinationVector[coloredVer[i].size() - 1] = 1;
			combinationVector[coloredVer[i].size() - 2] = 1;
			unsigned int cShPath = UINT_MAX; //shortest path between vertices in 'i' colour
			do {
				std::vector<unsigned int > values = getCombinationValues(combinationVector, coloredVer[i]);
				if (sp->getShrtsPaths()->getValue(values[0], values[1]) < cShPath) {
					cShPath = sp->getShrtsPaths()->getValue(values[0], values[1]);
				}

			} while (std::next_permutation(combinationVector.begin(), combinationVector.end()));

			if (pShPath > cShPath) {
				pShPath = cShPath;
			}



		}

		if (sPath <= pShPath) {
			sPath = pShPath + 1;
			retColoring = coloringTable;
		}



	} while (next_variation(coloringTable.begin(), coloringTable.end(), colourNumber));

	sPath = sPath - 1;
	optimalMinPath = sPath;
	return  retColoring;
}
std::vector<unsigned int> Algoritms::approximateAlgoritm()
{
	unsigned int size = gr->getV();
	std::vector <unsigned int> coloringTable(size, UINT_MAX);
	std::vector<unsigned int > retColoring;
	std::vector<std::vector<unsigned int>> coloredVertices(colourNumber, std::vector<unsigned int>());
	unsigned int actualCol = 0;
	unsigned int colVertCount = size;
	unsigned int colorUsedCount = 0;

	unsigned int maxPath = 0;
	unsigned int maxPathR;
	unsigned int maxPathC;
	int rowSize = 1;
	
	while (colVertCount > 0) {
		maxPath = 0;

		if (colVertCount <= (colourNumber - colorUsedCount)) {
			for (auto it = coloringTable.begin(); it != coloringTable.end(); ++it) {
				if (*it == UINT_MAX) {
					*it = actualCol++;
				}
			}
			break;
		}
		
		unsigned int pickedCol = actualCol;
		rowSize = 1;
		for (unsigned i = 0; i < size; ++i) {
			for (unsigned int j = 0; j < rowSize; ++j) {

				if (coloringTable[i] == UINT_MAX && coloringTable[j] == UINT_MAX) {
					if (sp->getShrtsPaths()->getValue(i, j) > maxPath) {
						maxPath = sp->getShrtsPaths()->getValue(i, j);
						maxPathC = j;
						maxPathR = i;
						pickedCol = actualCol;
					}
				}
				else if (coloringTable[i] == UINT_MAX) {
					unsigned int minPath = UINT_MAX;
					for (auto it = coloredVertices[coloringTable[j]].begin(); it != coloredVertices[coloringTable[j]].end(); it++) {
						if (sp->getShrtsPaths()->getValue(i, *it) < minPath) {
							minPath = sp->getShrtsPaths()->getValue(i, *it);
						}
					}
					if (minPath > maxPath) {
						maxPath = sp->getShrtsPaths()->getValue(i, j);
						maxPathC = j;
						maxPathR = i;
						pickedCol = coloringTable[j];
					}
				}
				else if (coloringTable[j] == UINT_MAX) {
					unsigned int minPath = UINT_MAX;
					for (auto it = coloredVertices[coloringTable[i]].begin(); it != coloredVertices[coloringTable[i]].end(); it++) {
						if (sp->getShrtsPaths()->getValue(i, *it) < minPath) {
							minPath = sp->getShrtsPaths()->getValue(*it, j);
						}
					}
					if (minPath > maxPath) {
						maxPath = sp->getShrtsPaths()->getValue(i, j);
						maxPathC = j;
						maxPathR = i;
						pickedCol = coloringTable[i];
					}
				}
				else {
					continue;
				}

			}
			++rowSize;
		}

		if (coloringTable[maxPathC] == UINT_MAX && coloringTable[maxPathR] == UINT_MAX) {
			sp->getShrtsPaths()->setValue(maxPathC, maxPathR, 0);
			coloringTable[maxPathC] = pickedCol;
			coloringTable[maxPathR] = pickedCol;
			coloredVertices[pickedCol].push_back(maxPathR);
			coloredVertices[pickedCol].push_back(maxPathC);
			actualCol = ((actualCol + 1) % colourNumber);
			colVertCount = colVertCount - 2;
			if (colorUsedCount != colourNumber) {
				colorUsedCount++;
			}
		}
		else if (coloringTable[maxPathC] == UINT_MAX) {
			sp->getShrtsPaths()->setValue(maxPathC, maxPathR, 0);
			coloringTable[maxPathC] = pickedCol;
			coloredVertices[pickedCol].push_back(maxPathC);
			colVertCount = colVertCount - 1;
		}
		else if (coloringTable[maxPathR] == UINT_MAX) {
			sp->getShrtsPaths()->setValue(maxPathC, maxPathR, 0);
			coloringTable[maxPathR] = pickedCol;
			coloredVertices[pickedCol].push_back(maxPathR);
			colVertCount = colVertCount - 1;
		}
	}

	sp->calculatePaths();

	unsigned int pShPath = UINT_MAX;
	for (int i = 0; i < colourNumber; ++i) {
		if (coloredVertices[i].empty() || coloredVertices[i].size() == 1) {
			continue;
		}

		std::vector<unsigned int> combinationVector(coloredVertices[i].size(), 0);
		combinationVector[coloredVertices[i].size() - 1] = 1;
		combinationVector[coloredVertices[i].size() - 2] = 1;
		unsigned int cShPath = UINT_MAX; //shortest path between vertices in 'i' colour
		do {
			std::vector<unsigned int > values = getCombinationValues(combinationVector, coloredVertices[i]);
			if (sp->getShrtsPaths()->getValue(values[0], values[1]) < cShPath) {
				cShPath = sp->getShrtsPaths()->getValue(values[0], values[1]);
			}

		} while (std::next_permutation(combinationVector.begin(), combinationVector.end()));

		if (pShPath > cShPath) {
			pShPath = cShPath;
		}
	}
	approxMinPath = pShPath;

	return coloringTable;
}
std::vector<unsigned int> Algoritms::getCombinationValues(std::vector<unsigned int> comb, std::vector<unsigned int> val) {
	std::vector<unsigned int> table;
	for (int i = 0; i < comb.size(); ++i) {
		if (comb[i] == 0) {
			continue;
		}
		table.push_back(val[i]);
	}
	return table;
}