#include "diamondSquare.h"

DiamondSquare::DiamondSquare(const int power2, long idum, const int RNG, const bool PBC):
    n(power2),
    power2(power2),
    systemLength(pow(2,power2)),
    idum(idum),
    R(zeros<mat>(systemLength+1,systemLength+1)),
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
        R(0,0)                       = ran2(&idum) - 0.5;
        R(0,systemLength)            = ran2(&idum) - 0.5;
        R(systemLength,0)            = ran2(&idum) - 0.5;
        R(systemLength,systemLength) = ran2(&idum) - 0.5;
    }
    else {
        R(0,0)                       = corners;
        R(0,systemLength)            = R(0,0);
        R(systemLength,0)            = R(0,0);
        R(systemLength,systemLength) = R(0,0);
    }

    double randrange = 1.0;
    double randfac = pow(2.0, -H);
    uint l;

    uint x[systemLength*systemLength/4];
    uint y[systemLength*systemLength/4];
    uint nSteps;
    uint stepLength = systemLength;
    uint halfStep = stepLength/2;

    cout << "systemLength = " << systemLength << endl;

    for (uint i = 0; i < power2; i++) {
        nSteps = systemLength/stepLength;

        l = 0;
        for (uint j = 0; j < nSteps; ++j) {
            for (uint k = 0; k < nSteps; ++k) {
                x[l] = j*stepLength;
                y[l] = k*stepLength;
                ++l;
            }
        }

        /* squares */
        for (uint j = 0; j < nSteps*nSteps; j++) {
            square(x[j], y[j], stepLength, halfStep, randrange, R);
        }

//        cout << endl << "R after squares" << endl << R << endl;

        /* diamonds */
        if (PBC) {
            for (uint j = 0; j < nSteps*nSteps; j++) {
                smartDiamond(x[j], y[j], stepLength, halfStep, systemLength, randrange, R);
            }
        } else {
            for (uint j = 0; j < nSteps*nSteps; j++) {
                diamond(x[j], y[j], stepLength, halfStep, randrange, R);
            }
        }

//        cout << endl << "R after diamonds" << endl << R << endl;

        if (PBC) {
            uint idx;
            for (uint j = 0; j < nSteps; j++) {
                idx = halfStep + j*stepLength;
                R(idx, systemLength) = R(idx,0);
                R(systemLength, idx) = R(0,idx);
            }
//            cout << "R after PBC" << endl << R << endl;
        } else {
            /* bottom diamonds */
            for (uint j = 0; j < nSteps; j++) {
                x[j] = systemLength - stepLength;
                y[j] = j*stepLength;
            }
            for (uint j = 0; j < nSteps; j++)
                bottom(x[j], y[j], stepLength, halfStep, randrange, R);

//            cout << endl << "R after bottom" << endl << R << endl;

            /* right diamonds */
            for (uint j = 0; j < nSteps; j++) {
                x[j] = j*stepLength;
                y[j] = systemLength - stepLength;
            }
            for (uint j = 0; j < nSteps; j++)
                right(x[j], y[j], stepLength, halfStep, randrange, R);

//            cout << endl << "R after right" << endl << R << endl;
        }

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
        const uint &stepSize,
        const uint &halfStep,
        const double &randrange,
        mat &R)
{
    /* left */
    xpos[0] = x;
    xpos[1] = x + stepSize;
    xpos[2] = x + halfStep;

    ypos[0] = y;
    ypos[1] = y;
    ypos[2] = y + halfStep;

    if (y == 0) { // at left edge
        nPoints = 3;
    } else {
        xpos[3] = x + halfStep;
        ypos[3] = y - halfStep;
        nPoints = 4;
    }

    sum = 0.0;
    for (uint i = 0; i < nPoints; i++) {
        sum += R(xpos[i], ypos[i]);
    }
    R(x + halfStep, y) += sum/nPoints + random(&idum)*randrange;


    /* top */
    ypos[0] = y;
    ypos[1] = halfStep;
    ypos[2] = stepSize;

    xpos[0] = x;
    xpos[1] = halfStep;
    xpos[2] = x;

    if (x == 0) {
        nPoints = 3;
    } else {
        xpos[3] = x - halfStep;
        ypos[3] = halfStep;
        nPoints = 4;
    }

    sum = 0.0;
    for (uint i = 0; i < nPoints; i++) {
        sum += R(xpos[i], ypos[i]);
    }
    R(x, y + halfStep) += sum/nPoints + random(&idum)*randrange;
}
void DiamondSquare::smartDiamond(
        const uint &x,
        const uint &y,
        const uint &stepSize,
        const uint &halfStep,
        const uint &systemLength,
        const double &randrange,
        mat &R)
{
    /* left */
    xpos[0] = x;
    xpos[1] = x + stepSize;
    xpos[2] = x + halfStep;

    ypos[0] = y;
    ypos[1] = y;
    ypos[2] = y + halfStep;

    if (y == 0) { // at left edge
        if (PBC) {
            xpos[3] = x + halfStep;
            ypos[3] = systemLength - halfStep;
            nPoints = 4;
        }
        else {
            nPoints = 3;
        }
    } else {
        xpos[3] = x + halfStep;
        ypos[3] = y - halfStep;
        nPoints = 4;
    }

    sum = 0.0;
    for (uint i = 0; i < nPoints; i++) {
        sum += R(xpos[i], ypos[i]);
    }
    R(x + halfStep, y) += sum/nPoints + random(&idum)*randrange;


    /* top */
    ypos[0] = y;
    ypos[1] = halfStep;
    ypos[2] = stepSize;

    xpos[0] = x;
    xpos[1] = halfStep;
    xpos[2] = x;

    if (x == 0) {
        if (PBC) {
            xpos[3] = systemLength - halfStep;
            ypos[3] = halfStep;
            nPoints = 4;
        }
        else {
            nPoints = 3;
        }
    } else {
        xpos[3] = x - halfStep;
        ypos[3] = halfStep;
        nPoints = 4;
    }

    sum = 0.0;
    for (uint i = 0; i < nPoints; i++) {
        sum += R(xpos[i], ypos[i]);
    }
    R(x, y + halfStep) += sum/nPoints + random(&idum)*randrange;
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
