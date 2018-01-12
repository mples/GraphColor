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

int main(int argc, char** argv) {

    if(argc != 3 ){
        std::cout << "Wrong arguments\n";
        return 0;
    }
    int V = std::atoi(argv[1]);
    int E = std::atoi(argv[2]);
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
        file << std::rand() % V;
        file << ' ';
        file << std::rand() % V;
        file << ' ';
        file << std::rand() % 10;
        file << '\n';
    }
    file.close();




    //Graph gr("gengr.txt");
	Graph gr("graf1.txt");
    //Graph gr("/home/mateusz/CLionProjects/GraphGen/graf.txt");
    //std::cout << gr.loadFile("/home/mateusz/CLionProjects/GraphGen/graf.txt");

    std::cout << std::endl;
    std::cout << gr.getE() << ' ' << gr.getV() << std::endl;
    std::cout << gr.getMatrix()->printMatrix();

	std::cout << "Shortest paths" << std::endl;
	ShortestPaths sp( &gr);
	sp.calculatePaths();
	std::cout << sp.getShrtsPaths()->printMatrix();
	
	std::vector<unsigned int > solved = optimalAlgoritm(&gr, 2, &sp);
	for (int i = 0; i < solved.size(); ++i) {
		std::cout << i << ' ';
		std::cout << solved[i] << std::endl;
	}
	
	std::getchar();
    return 0;
}