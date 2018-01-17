#include <iostream>
#include <stdlib.h>
#include <ctime>
#include <fstream>
#include <algorithm>
#include <vector>
#include "Graph.h"	
#include "ShortestPaths.h"

template<class _Tnumber, class _Titerator >
bool next_variation
(
	_Titerator const& _First
	, _Titerator const& _Last
	, _Tnumber const& _Upper
	, _Tnumber const& _Start = 0
	, _Tnumber const& _Step = 1
)
{
	_Titerator _Next = _First;
	while (_Next != _Last)
	{
		*_Next += _Step;
		if (*_Next < _Upper)
		{
			return true;
		}
		(*_Next) = _Start;
		++_Next;
	}
	return false;
}

std::vector<unsigned int> getCombinationValues(std::vector<unsigned int> comb, std::vector<unsigned int> val) {
	std::vector<unsigned int> table;
	for (int i = 0; i < comb.size(); ++i) {
		if (comb[i] == 0) {
			continue;
		}
		table.push_back(val[i]);
	}
	return table;
}

std::vector<unsigned int > optimalAlgoritm(Graph* gr, unsigned int colourNumber,ShortestPaths *sp) {
	unsigned int size = gr->getV();
	std::vector <unsigned int> coloringTable (size,0);
	unsigned int sPath = 1;
	std::vector<unsigned int > retColoring;
	do {
		unsigned int pShPath = UINT_MAX; 
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
			combinationVector[coloredVer[i].size() - 2 ] = 1;
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
		


	} while (next_variation(coloringTable.begin(), coloringTable.end(),colourNumber));

	sPath = sPath - 1;
	std::cout << std::endl << sPath << std::endl;
	return  retColoring;

}

std::vector<unsigned int > approximateAlgoritm(Graph* gr, unsigned int colourNumber, ShortestPaths *sp) {
	unsigned int size = gr->getV();
	std::vector <unsigned int> coloringTable(size,UINT_MAX);
	std::vector<unsigned int > retColoring;
	std::vector<std::vector<unsigned int>> coloredVertices(colourNumber, std::vector<unsigned int>());
	unsigned int actualCol = 0;
	unsigned int colVertCount = size;
	unsigned int colorUsedCount = 0;

	unsigned int maxPath = 0;
	unsigned int maxPathR;
	unsigned int maxPathC;
	int rowSize = 1;
	for (unsigned  i = 0; i < size; ++i) {
		for (unsigned int j = 0; j < rowSize; ++j) {
			if (sp->getShrtsPaths()->getValue(i, j) == UINT_MAX) {
				maxPath = UINT_MAX;
				maxPathC = j;
				maxPathR = i;
			}
			if (sp->getShrtsPaths()->getValue(i, j) > maxPath) {
				maxPath = sp->getShrtsPaths()->getValue(i, j);
				maxPathC = j;
				maxPathR = i;
			}
		}
		++rowSize;
	}

	sp->getShrtsPaths()->setValue(maxPathC, maxPathR, 0);
	coloredVertices[actualCol].push_back(maxPathR);
	coloredVertices[actualCol].push_back(maxPathC);
	coloringTable[maxPathC] = actualCol;
	coloringTable[maxPathR] = actualCol;
	actualCol = ((actualCol + 1) % colourNumber);
	colorUsedCount++;
	colVertCount = colVertCount - 2;

	while(colVertCount > 0 ) {
		maxPath = 0;
		
		if (colVertCount == (colourNumber - colorUsedCount)) {
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
				else if(coloringTable[i] == UINT_MAX) {
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

	
	return coloringTable;
}

unsigned int getShrtPath(unsigned int colourNumber, ShortestPaths *sp, std::vector<unsigned int> coloring) {
	std::vector<std::vector<unsigned int>> coloredVertices(colourNumber, std::vector<unsigned int>());
	for (unsigned int i = 0; i < coloring.size(); ++i) {
		coloredVertices[coloring[i]].push_back(i);
	}

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
	return pShPath;
}

int main(int argc, char** argv) {

    if(argc != 3 ){
        std::cout << "Wrong arguments\n";
        return 0;
    }
    //int V = std::atoi(argv[1]);
    //int E = std::atoi(argv[2]);
	int V = 7;
	int E = 100;
	std::cout << V << ' ' << E << std::endl;
    srand(time(NULL));
    std::ofstream file;
    file.open("gengr.txt");
    if( !file.good() ){
        return 0;
    }
    file << V << ' ';
    //file << E << ' '<< '\n';
	file << E << '\n';


    for (int i =0; i < E ; ++i){
		unsigned int  v1;
		unsigned int  v2;
		do {
			v1 = std::rand() % V;
			v2 = std::rand() % V;
		} while (v1 == v2);

        file << v1;
        file << ' ';
        file << v2;
        file << ' ';
        file << std::rand() % 10;
        file << '\n';
    }
    file.close();




    Graph gr("gengr.txt");
	//Graph gr("graf.txt");
    //Graph gr("/home/mateusz/CLionProjects/GraphGen/graf.txt");
    //std::cout << gr.loadFile("/home/mateusz/CLionProjects/GraphGen/graf.txt");

    std::cout << std::endl;
    std::cout << gr.getE() << ' ' << gr.getV() << std::endl;
    std::cout << gr.getMatrix()->printMatrix();

	std::cout << "Shortest paths" << std::endl;
	ShortestPaths sp( &gr);
	ShortestPaths sp2(&gr);
	sp.calculatePaths();
	sp2.calculatePaths();
	std::cout << sp.getShrtsPaths()->printMatrix();
	
	std::vector<unsigned int > solved = optimalAlgoritm(&gr, 5, &sp);
	for (int i = 0; i < solved.size(); ++i) {
		std::cout << i << ' ';
		std::cout << solved[i] << std::endl;
	}
	std::cout << std::endl;
	//std::cout << getShrtPath(5, &sp, solved);
	std::cout << std::endl;
	std::cout << std::endl;
	solved = approximateAlgoritm(&gr, 5, &sp);
	for (int i = 0; i < solved.size(); ++i) {
		 std::cout << i << ' ';
		 std::cout << solved[i] << std::endl;
	}

	std::cout << std::endl;
	std::cout << getShrtPath(5,&sp2, solved);

	std::getchar();
    return 0;
}