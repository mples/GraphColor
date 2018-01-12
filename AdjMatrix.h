//
// Created by mateusz on 30.12.17.
//

#ifndef GRAPHGEN_ADJMATRIX_H
#define GRAPHGEN_ADJMATRIX_H

#include <vector>
#include <string>

class AdjMatrix {

public:
    AdjMatrix(unsigned int s);
	AdjMatrix(unsigned int s, unsigned int defValue);
    ~AdjMatrix();
    void setValue(unsigned int r, unsigned int c, unsigned int val);
    unsigned int getValue(unsigned int r, unsigned int c);
	unsigned int getSize();
    std::string printMatrix();

protected:

private:
    unsigned int ** Matrix;
    unsigned int Size;
    void fill(unsigned int defValue);


};


#endif //GRAPHGEN_ADJMATRIX_H
