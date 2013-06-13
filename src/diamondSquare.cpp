#include "diamondSquare.h"

DiamondSquare::DiamondSquare(const int power2, long idum, const int RNG, const bool PBC):
    n(power2),
    power2(power2),
    L0(pow(2,power2)),
    L(L0),
    idum(idum),
    R(zeros<mat>(L0+1,L0+1)),
    RNG(RNG),
    PBC(PBC)
{
    if (RNG > 2) {
        cout << "RNG too large, have only implemented 3 random number generators" << endl;
        exit(1);
    }
}

mat DiamondSquare::generate(const double H, const double corners)
{
    if (corners == 42) {
        R(0,0)   = ran2(&idum) - 0.5;
        R(0,L0)  = ran2(&idum) - 0.5;
        R(L0,0)  = ran2(&idum) - 0.5;
        R(L0,L0) = ran2(&idum) - 0.5;
    }
    else {
        R(0,0) = corners;
        R(0,L0) = R(0,0);
        R(L0,0) = R(0,0);
        R(L0,L0) = R(0,0);
    }

    double randrange = 1.0;
    double randfac = pow(2.0, -H);
    uint l;

    uint x[L0];
    uint y[L0];
    uint nSteps;
    uint stepLength = L0;
    uint halfStep = stepLength/2;

    for (uint i = 0; i < power2; i++) {
        nSteps = L0/stepLength;

//        cout << "stepLength = " << stepLength << endl;
//        cout << "nSteps = " << nSteps << endl;

        l = 0;
        for (uint j = 0; j < nSteps; j++) {
            for (uint k = 0; k < nSteps; k++) {
                x[l] = j*nSteps;
                y[l] = k*nSteps;
                l++;
            }
        }

//        for (uint j = 0; j < nSteps*nSteps; j++)
//            cout << "x[" << j << "] = " << x[j] << endl;
//        for (uint j = 0; j < nSteps*nSteps; j++)
//            cout << "y[" << j << "] = " << y[j] << endl;

        /* squares */
        for (uint j = 0; j < nSteps*nSteps; j++) {
            square(x[j], y[j], stepLength, halfStep, randrange, R);
        }

//        cout << endl << "R after squares" << endl << R << endl;

        /* diamonds */
        for (uint j = 0; j < nSteps*nSteps; j++) {
            diamond(x[j], y[j], stepLength, halfStep, randrange, R);
        }

//        cout << endl << "R after diamonds" << endl << R << endl;

        /* bottom diamonds */
        for (uint j = 0; j < nSteps; j++) {
            x[j] = L0 - stepLength;
            y[j] = j*stepLength;
        }
        for (uint j = 0; j < nSteps; j++)
            bottom(x[j], y[j], stepLength, halfStep, randrange, R);

//        cout << endl << "R after bottom" << endl << R << endl;

        /* right diamonds */
        for (uint j = 0; j < nSteps; j++) {
            x[j] = j*stepLength;
            y[j] = L0 - stepLength;
        }
        for (uint j = 0; j < nSteps; j++)
            right(x[j], y[j], stepLength, halfStep, randrange, R);

//        cout << endl << "R after right" << endl << R << endl;

        stepLength /= 2;
        halfStep /= 2;
        randrange *= randfac;
    }

    return R;
}

void DiamondSquare::square(
        const uint &x,
        const uint &y,
        const uint &L,
        const uint &Lhalf,
        const double &randrange,
        mat &R)
{
    for (uint i = 0; i < 4; i++) {
        xpos[i] = x;
        ypos[i] = y;
    }
    xpos[2] += L;
    xpos[3] += L;
//    ypos(0) += 0.0;
    ypos[1] += L;
//    ypos(2) += 0.0;
    ypos[3] += L;

    sum = 0.0;
    for (int i = 0; i < 4; i++) {
        sum += R(xpos[i], ypos[i]);
    }
    R(x + Lhalf, y + Lhalf) += sum*0.25 + random(&idum)*randrange; // change to = instead of +=
}

void DiamondSquare::diamond(
        const uint &x,
        const uint &y,
        const uint &L,
        const uint &Lhalf,
        const double &randrange,
        mat &R)
{
    uint nPoints;

    /* left */
    nPoints = 4;
    for (uint i = 0; i < 4; i++) {
        xpos[i] = x;
        ypos[i] = y;
    }

    xpos[0] += Lhalf; // OK WITH BOUNDARIES
    xpos[1] += Lhalf;
    xpos[2] += 0;
    xpos[3] += L;

    if (y == 0) { // BOUNDARIES
        nPoints = 3;
        ypos[0] = 0;
    } else {
        ypos[0] = y - Lhalf;
    }
    ypos[1] += Lhalf;
    ypos[2] += 0;
    ypos[3] += 0;

    sum = 0.0;
    for (uint i = 0; i < 4; i++) {
        sum += R(xpos[i], ypos[i]);
    }
    R(x + Lhalf, y) += sum/nPoints + random(&idum)*randrange;

    /* top */
    nPoints = 4;
    for (int i = 0; i < 4; i++) {
        xpos[i] = x;
        ypos[i] = y;
    }

    if (x == 0) {
        xpos[0] = 0;
        nPoints = 3;
    } else {
        xpos[0] = x - Lhalf;
    }
//    xpos(0) = (pos(0) == 0) ? pos(0) : pos(0) - L/2; // boundaries
    xpos[1] += Lhalf;
    xpos[2] += 0;
    xpos[3] += 0;

    ypos[0] += Lhalf; // OK with boundaries
    ypos[1] += Lhalf;
    ypos[2] += 0;
    ypos[3] += L;

    sum = 0.0;
    for (int i = 0; i < 4; i++) {
        sum += R(xpos[i], ypos[i]);
    }
    R(x, y + Lhalf) += sum/nPoints + random(&idum)*randrange;
}

void DiamondSquare::bottom(
        const uint &x,
        const uint &y,
        const uint &L,
        const uint &Lhalf,
        const double &randrange,
        mat &R)
{
    /* bottom */
    for (int i = 0; i < 3; i++) {
        xpos[i] = x;
        ypos[i] = y;
    }

    xpos[0] += L;
    xpos[1] += Lhalf;
    xpos[2] += L;

    ypos[0] += 0;
    ypos[1] += Lhalf;
    ypos[2] += L;

    sum = 0.0;
    for (int i = 0; i < 3; i++) {
        sum += R(xpos[i], ypos[i]);
    }
    R(x + L, y + Lhalf) += sum/3.0 + random(&idum)*randrange;
}

void DiamondSquare::right(
        const uint &x,
        const uint &y,
        const uint &L,
        const uint &Lhalf,
        const double &randrange,
        mat &R)
{
    /* right */
    for (uint i = 0; i < 3; i++) {
        xpos[i] = x;
        ypos[i] = y;
    }

    xpos[0] += 0; // OK WITH BOUNDARIES
    xpos[1] += Lhalf;
    xpos[2] += L;

    ypos[0] += L;
    ypos[1] += Lhalf;
    ypos[2] += L;

    sum = 0.0;
    for (uint i = 0; i < 3; i++) {
        sum += R(xpos[i], ypos[i]);
    }
    R(x + Lhalf, y + L) += sum/3.0 + random(&idum)*randrange;
}
