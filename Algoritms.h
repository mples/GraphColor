#pragma once
#include <vector>
#include <algorithm>
#include <iostream>
#include "Graph.h"
#include "ShortestPaths.h"


class Algoritms
{
public:
	Graph * gr;
	ShortestPaths* sp;
	unsigned int colourNumber;
	unsigned int optimalMinPath;
	unsigned int approxMinPath;
	Algoritms(std::string fname, unsigned int cNum);
	~Algoritms();
	std::vector<unsigned int > optimalAlgoritm();
	std::vector<unsigned int > approximateAlgoritm();

private:

	template<class _Tnumber, class _Titerator >
	bool next_variation
	(
		_Titerator const& _First
		, _Titerator const& _Last
		, _Tnumber const& _Upper
		, _Tnumber const& _Start = 0
		, _Tnumber const& _Step = 1
	);
	std::vector<unsigned int> getCombinationValues(std::vector<unsigned int> comb, std::vector<unsigned int> val);
};

template<class _Tnumber, class _Titerator>
inline bool Algoritms::next_variation(_Titerator const & _First, _Titerator const & _Last, _Tnumber const & _Upper, _Tnumber const & _Start, _Tnumber const & _Step)
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
