//
// Created by mateusz on 30.12.17.
//

#include "AdjMatrix.h"

AdjMatrix::AdjMatrix(unsigned int s) {
    Size = s;
    Matrix = new unsigned int * [Size];
    for (int i = 0 ; i < Size ; ++i){
        Matrix[i] = new unsigned int[i+1];
    }
}

AdjMatrix::AdjMatrix(unsigned int s, unsigned int defValue)
{
	Size = s;
	Matrix = new unsigned int *[Size];
	for (int i = 0; i < Size; ++i) {
		Matrix[i] = new unsigned int[i + 1];
	}
	fill(defValue);
}

AdjMatrix::~AdjMatrix() {
    for( int i =0 ; i < Size ; ++i){
        delete[] Matrix[i];
    }
    delete[] Matrix;
}

void AdjMatrix::fill(unsigned int defValue) {

    int rowSize = 1;
    for (int i = 0 ; i < Size ; ++i){
        for(int j = 0 ; j < rowSize ; ++j){
            Matrix[i][j] = defValue;
        }
        ++rowSize;
    }
}

void AdjMatrix::setValue(unsigned int r, unsigned int c, unsigned int val) {
    if( c > r){
        Matrix[c][r] = val;
    }
    else{
        Matrix[r][c] = val;
    }

}

unsigned int AdjMatrix::getValue(unsigned int r, unsigned int c) {
    if (c > r){
        return Matrix[c][r];
    }
    else{
        return Matrix[r][c];
    }
}

unsigned int AdjMatrix::getSize()
{
	return Size;
}

std::string AdjMatrix::printMatrix() {

    std::string ret;
    int rowSize = 1;
    for(int i = 0 ; i < Size ; ++i){
        for(int j = 0 ; j < rowSize ; ++j){
            ret = ret + std::to_string(Matrix[i][j]);
            ret+= ' ';
        }
        ret += '\n';
        ++rowSize;
    }
    return ret;
}
