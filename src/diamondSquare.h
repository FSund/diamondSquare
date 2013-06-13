#ifndef DIAMONDSQUARE_H
#define DIAMONDSQUARE_H

#include <iostream>
#include <armadillo>
#include "lib.h"

using namespace std;
using namespace arma;

class DiamondSquare {
public:
    DiamondSquare(const int power2, long idum, const int RNG = 1, const bool PBC = false);
    mat generate(const double H, const double corners = 42);
private:
    void square(const uint &x, const uint &y, const uint &L, const uint &Lhalf, const double &randrange, mat &R);
    void diamond(const uint &x, const uint &y, const uint &L, const uint &Lhalf, const double &randrange, mat &R);

    void bottom(const uint &x, const uint &y, const uint &L, const uint &Lhalf, const double &randrange, mat &R);
    void right(const uint &x, const uint &y, const uint &L, const uint &Lhalf, const double &randrange, mat &R);

    double random(long *seed);
    double gaussianDeviate(long *seed);

    uint n, power2, L0, L;
    long idum;
    mat R;
    const int RNG;
    const bool PBC;

    uint xpos[4];
    uint ypos[4];
    double sum;
};

inline double DiamondSquare::random(long *seed)
{
    /* returns random number with mean 0 */
    if (RNG == 0)
        return 0.0;
    else if (RNG == 1)
        return (ran2(seed) - 0.5);
    else if (RNG == 2)
        return gaussianDeviate(seed);
    else
        return NAN;
}

inline double DiamondSquare::gaussianDeviate(long *seed)
{
    /* returns normally distributed random number (mean 0, stddev 1) */
    double R, randomNormal;
    // Box-Muller transform
    R = sqrt(-2.0*log(ran2(seed)));
    randomNormal = R*cos(2.0*pi*ran2(seed));

    return randomNormal;
}

#endif // DIAMONDSQUARE_H
