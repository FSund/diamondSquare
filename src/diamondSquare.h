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
            const vec corners,
            const long seed,
            const double sigma,
            const bool addition,
            const bool PBC,
            const uint RNG);

private:
    void runDiamondSquare(mat &R, const double H, const double sigma);
    double square(const uint x, const uint y, const uint halfStepLength, const double RNGstddv, const mat &R);
    double diamond(const uint x, const uint y, const uint halfStepLength, const double RNGstddv, const mat &R);

    double bottomEdgeDiamonds(const uint x, const uint y, const uint halfStepLength, const double RNGstddv, mat &R);
    double rightEdgeDiamonds(const uint x, const uint y, const uint halfStepLength, const double RNGstddv, mat &R);

    double random();
    double gaussianDeviate(long *seed);

    mat R;

    uint power2, systemSize, zerolength;
    int RNG;
    bool PBC;
    double sigma;
    bool addition;
};

#endif // DIAMONDSQUARE_H
