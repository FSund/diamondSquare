#ifndef DIAMONDSQUARE_H
#define DIAMONDSQUARE_H

#include <iostream>

#include <armadillo>

using namespace std;
using namespace arma;

class DiamondSquare {
public:
    mat& generate(const uint power2,
            const double H,
            const double minZValue,
            const double maxZValue,
            const bool PBC, const long idum,
            const uint RNG);

private:
    void runDiamondSquare(mat &R, const double H);
    void square(const uint x, const uint y, const uint squareSize, const uint squareSizeHalf, const double RNGstddv, mat &R);
    void diamond(const uint x, const uint y, const uint stepLength, const uint halfStepLength, const double RNGstddv, mat &R);

    void bottomEdgeDiamonds(const uint x, const uint y, const uint halfStepLength, const double RNGstddv, mat &R);
    void rightEdgeDiamonds(const uint x, const uint y, const uint halfStepLength, const double RNGstddv, mat &R);

    double random();
    double gaussianDeviate(long *seed);

    uint power2, systemSize;
    long idum;
    mat R;
    int RNG;
    bool PBC;
};

#endif // DIAMONDSQUARE_H
