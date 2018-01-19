#include <iostream>
#include <stdlib.h>
#include <ctime>
#include <fstream>
#include <algorithm>
#include <vector>
#include <string>
#include <chrono>
#include <iomanip>
#include <math.h>


#include "cxxopts.hpp"
#include "Graph.h"	
#include "ShortestPaths.h"
#include "Algoritms.h"

#define DEF_FNAME "gengr.txt"

int genGraph(unsigned int V , unsigned int E, std::string fname) {
	srand(time(NULL));
	std::ofstream file;
	file.open(fname);
	if (!file.good()) {
		return 0;
	}
	file << V << ' ';
	file << E << '\n';


	for (int i = 0; i < E; ++i) {
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
		file << std::rand() % 100;
		file << '\n';
	}
	file.close();
}

void firstMode(std::string f, unsigned int N) {

	Algoritms alg(f, N);
	std::cout << "Graph adjacency matrix: " << std::endl;
	std::cout << alg.gr->getMatrix()->printMatrix() << std::endl;
	std::cout << "Shortest Paths between all vertices:" << std::endl;
	std::cout << alg.sp->getShrtsPaths()->printMatrix() << std::endl;
	std::vector<unsigned int> optimalColoring = alg.optimalAlgoritm();
	std::cout << "Optimal minimal same color path:" << alg.optimalMinPath<< std::endl;
	for (int i = 0; i < optimalColoring.size(); ++i) {
		std::cout << i << "->" << optimalColoring[i] << std::endl;
	}
	std::vector<unsigned int > approxColoring = alg.approximateAlgoritm();
	std::cout << "Approximate minimal same color path:" << alg.approxMinPath << std::endl;
	for (int i = 0; i < optimalColoring.size(); ++i) {
		std::cout << i << "->" << approxColoring[i] << std::endl;
	}
}
void secondMode(unsigned int V, unsigned int E, unsigned int N, std::string f) {
	if (f.empty()) {
		genGraph(V, E, DEF_FNAME);
		firstMode(DEF_FNAME, N);
	}
	else {
		genGraph(V, E, f);
		firstMode(f, N);
	}

}
void thirdMode(unsigned int V, unsigned int E, unsigned int N, unsigned int k, unsigned int r, unsigned int s) {
	unsigned int nProb = V;
	double avrTime = 0, timeCounter = 0;
	std::vector<double> timeResultsOptimal;
	std::vector<double> timeResultsApprox;
	double cOptimal;
	double cApprox;

	for (int i = 0; i < k; ++i, nProb += s) {
		avrTime = 0;
		for (int j = 0; j < r; ++j) {
			genGraph(nProb, E, DEF_FNAME);
			Algoritms alg(DEF_FNAME, N);
			auto start = std::chrono::system_clock::now();

			alg.optimalAlgoritm();

			auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(
				std::chrono::system_clock::now() - start);
			timeCounter = milliseconds.count();
			avrTime = avrTime + timeCounter;
		}
		avrTime = avrTime / r;
		timeResultsOptimal.push_back(avrTime);
		
	}

	double c3 = ((int)(k / 2)*s);
	double c1 = std::pow((double)(V + ((int)(k / 2)*s)), (double)N);
	double c2 = timeResultsOptimal[(int)(k / 2)];
	cOptimal = timeResultsOptimal[(int)(k / 2)] / std::pow((double)(V + ((int)(k / 2)*s)), (double)N);
	std::cout << cOptimal << std::endl;

	nProb = V;
	for (int i = 0; i < k; ++i, nProb += s) {
		double t = timeResultsOptimal[i] / (cOptimal * std::pow(nProb, N));

		std::cout << "| ";
		std::cout << std::setw(8) << nProb << " | ";
		std::cout << std::fixed << std::setprecision(3) << std::setw(8) << timeResultsOptimal[i] << " | ";
		std::cout << std::fixed << std::setprecision(3) << std::setw(8) << t << " | ";
		std::cout << std::endl;
	}

	//Test for approximate algoritm

	nProb = V;
	for (int i = 0; i < k; ++i, nProb += s) {
		avrTime = 0;
		for (int j = 0; j < r; ++j) {
			genGraph(nProb, E, DEF_FNAME);
			Algoritms alg(DEF_FNAME, N);
			auto start = std::chrono::system_clock::now();

			alg.approximateAlgoritm();

			auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(
				std::chrono::system_clock::now() - start);
			
			timeCounter = milliseconds.count();
			avrTime = avrTime + timeCounter;
		}
		avrTime = avrTime / r;
		timeResultsApprox.push_back(avrTime);
	}

	cApprox = timeResultsApprox[(int)(k / 2)] / std::pow((double)(V + ((int)(k / 2)*s)), (double)N);
	std::cout << cApprox << std::endl;

	nProb = V;
	for (int i = 0; i < k; ++i, nProb += s) {
		double t = timeResultsApprox[i] / (cApprox * std::pow(nProb, N));

		std::cout << "| ";
		std::cout << std::setw(8) << nProb << " | ";
		std::cout << std::fixed << std::setprecision(3) << std::setw(8) << timeResultsApprox[i] << " | ";
		std::cout << std::fixed << std::setprecision(3) << std::setw(8) << t << " | ";
		std::cout << std::endl;
	}
}

int main(int argc, char** argv) {

	unsigned int m, V, E, N,k,r,s;
	std::string f;


	try {
		cxxopts::Options options(argv[0], " - command line options");
		options
			.positional_help("[optional args]")
			.show_positional_help();

		options.add_options("Program")
			("m", "Get mode to run", cxxopts::value<int>())
			("V, vertices", "Get number of vertices in graph", cxxopts::value<int>())
			("E, edges", "Get number of edges in graph", cxxopts::value<int>())
			("N, colors", "Get number of colors to paint graph", cxxopts::value<int>())
			("f, file", "Get file name for input", cxxopts::value<std::string>())
			("s, step", "Get step of tasks in next iterations", cxxopts::value<int>())
			("k, kproblem", "Get number of problems ", cxxopts::value<int>())
			("r, rinstance", "Get number of problem instantions", cxxopts::value<int>())
			("h, help", "Print help");

		
		auto result = options.parse(argc, argv);

		if (result.count("m")) {
			m = result["m"].as<int>();
			std::cout << "Mode "<< m << " launched" << std::endl;
		}
		if (result.count("N")) {
			N = result["N"].as<int>();
			std::cout << "N -> " << N << std::endl;
		}

		if (result.count("V")) {
			V = result["V"].as<int>();
			std::cout << "V -> " << V << std::endl;
		}

		if (result.count("E")) {
			E = result["E"].as<int>();
			std::cout << "E -> " << E << std::endl;
		}
		if (result.count("s")) {
			s = result["s"].as<int>();
			std::cout << "s -> " << s << std::endl;
		}

		if (result.count("r")) {
			r = result["r"].as<int>();
			std::cout << "r -> " << r << std::endl;
		}

		if (result.count("k")) {
			k = result["k"].as<int>();
			std::cout << "k -> " << k << std::endl;
		}

		if (result.count("f")) {
			f = result["f"].as<std::string>();
			std::cout << "f -> " << f << std::endl;
		}
	}
	catch (const cxxopts::OptionException &e) {
		std::cout << "Error in parsing options: " << e.what() << std::endl;
		exit(1);
	}

	switch (m) {
	case 1:
		firstMode(f,N);
		break;
	case 2:
		secondMode(V, E, N, f);
		break;
	case 3:
		thirdMode(V,E, N, k, r,s);
		break;
	default:
		break;
	}

	std::getchar();
    return 0;
}