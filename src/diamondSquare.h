#ifndef DIAMONDSQUARE_H
#define DIAMONDSQUARE_H

#include <iostream>
#include <armadillo>
#include "lib.h"

using namespace std;
using namespace arma;

class DiamondSquare {
public:
    DiamondSquare(const int power2, long idum);
    mat generate(const double H, const double corners = 42);
private:
    void square(mat &R, const uvec &pos, const int &L0, const int &L, const double &randrange);
    void diamond(mat &R, const uvec pos, const int L0, const int L, const double randrange);

    void bottom(mat &R, const uvec pos, const int L0, const int L, const double randrange);
    void right(mat &R, const uvec pos, const int L0, const int L, const double randrange);

//    int periodic(int idx, const int l);
//    umat periodic(umat idx, const int l);

    int n, L0, L;
    long idum;
    mat R;
};

#endif // DIAMONDSQUARE_H
