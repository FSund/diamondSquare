#include "diamondSquare.h"

mat &DiamondSquare::generate(
        const uint power2,
        const double H,
        const double minZValue,
        const double maxZValue,
        const bool PBC,
        const long idum,
        const uint RNG) {
    this->power2 = power2;
    this->PBC = PBC;
    this->RNG = RNG;
    systemSize = pow(2.0, power2) + 1;

    srand(idum); // setting the seed of the RNG for both C++'s rand()/srand() and Armadillo's randu()/randn()

    R = zeros<mat>(systemSize, systemSize);
    if (PBC) { // We need the same value in the corners if we are using periodic boundaries
        R(0,0)                       = randu<double>() - 0.5;
        R(0,systemSize-1)            = R(0,0);
        R(systemSize-1,0)            = R(0,0);
        R(systemSize-1,systemSize-1) = R(0,0);
    } else {
        R(0,0)                       = randu<double>() - 0.5;
        R(0,systemSize-1)            = randu<double>() - 0.5;
        R(systemSize-1,0)            = randu<double>() - 0.5;
        R(systemSize-1,systemSize-1) = randu<double>() - 0.5;
    }

    runDiamondSquare(R, H);

    if (RNG == 0 && PBC == 0) {
        return R;
    }

    // normalize to range [minZ maxZ]
    double minValue = min(min(R));
    double normFactor = 1.0/(max(max(R)) - minValue);
    for (uint i = 0; i < systemSize; i++) {
        for (uint j = 0; j < systemSize; j++) {
            R(i,j) = (R(i,j) - minValue)*normFactor*(maxZValue - minZValue) + minZValue;
        }
    }

    return R;
}

void DiamondSquare::runDiamondSquare(mat& R, const double H) {

    double RNGstddv;

    uint stepLength = systemSize-1;
    uint halfStepLength = stepLength/2;

    for (uint iteration = 0; iteration < 2*power2; iteration+=2) { // Doing two iterations inside the loop
        uint nSteps = (systemSize-1)/stepLength;

        // Squares
        RNGstddv = sqrt(1.0/pow(2.0, 2.0*H*iteration));
        for (uint x = 0; x < nSteps*stepLength; x += stepLength) {
            for (uint y = 0; y < nSteps*stepLength; y += stepLength) {
                square(x, y, stepLength, halfStepLength, RNGstddv, R);
            }
        }
//        cout << "R after squares" << endl << R << endl;

        // Diamonds
        RNGstddv = sqrt(1.0/pow(2.0, 2.0*H*(iteration+1)));
        for (uint x = 0; x < nSteps*stepLength; x += stepLength) {
            for (uint y = 0; y < nSteps*stepLength; y += stepLength) {
                diamond(x, y, stepLength, halfStepLength, RNGstddv, R);
            }
        }
//        cout << "R after diamonds" << endl << R << endl;

        if (PBC) {
            for (uint i = 0; i < nSteps; i++) {
                uint idx = halfStepLength + i*stepLength;
                R(idx, systemSize-1) = R(idx,0);
                R(systemSize-1, idx) = R(0,idx);
            }
//            cout << "R after PBC" << endl << R << endl;
        } else {
            // We have to do the bottom and right edge diamonds manually

            // Bottom edge diamonds
            for (uint y = halfStepLength; y < nSteps*stepLength; y += stepLength) {
                uint x = (nSteps-1)*stepLength + halfStepLength;
                bottomEdgeDiamonds(x, y, halfStepLength, RNGstddv, R);
            }
//            cout << "R after bottom diamonds" << endl << R << endl;

            // Right edge diamonds
            for (uint x = halfStepLength; x < nSteps*stepLength; x+= stepLength) {
                uint y = (nSteps-1)*stepLength + halfStepLength;
                rightEdgeDiamonds(x, y, halfStepLength, RNGstddv, R);
            }
//            cout << "R after right diamonds" << endl << R << endl;
        }

        stepLength /= 2;
        halfStepLength /= 2;
    }
}

void DiamondSquare::square(
        const uint x,
        const uint y,
        const uint stepLength,
        const uint halfStepLength,
        const double RNGstddv,
        mat &R) {

    double average = 0.25*(R(x, y) + R(x+stepLength, y) + R(x, y+stepLength) + R(x+stepLength, y+stepLength));
    R(x + halfStepLength, y + halfStepLength) = average + random()*RNGstddv; // change to = instead of +=
}

void DiamondSquare::diamond(
        const uint x,
        const uint y,
        const uint stepLength,
        const uint halfStepLength,
        const double RNGstddv,
        mat &R) {

    double average;

    // Point centered at left edge of square
    if (y == 0) { // At left edge of system
        if (PBC) {
            average = 0.25*(
                R(x, y) +
                R(x+stepLength, y) +
                R(x+halfStepLength, y+halfStepLength) +
                R(x+halfStepLength, systemSize - 1 - halfStepLength));
        } else {
            average = 0.33333333333333333333333*(
                R(x, y) +
                R(x+stepLength, y) +
                R(x+halfStepLength, y+halfStepLength));
        }
    } else { // Inside the system -- nothing to worry about
        average = 0.25*(
            R(x, y) +
            R(x+stepLength, y) +
            R(x+halfStepLength, y+halfStepLength) +
            R(x+halfStepLength, y-halfStepLength));
    }
    R(x + halfStepLength, y) = average + random()*RNGstddv;

    // Point centered at top edge of square
    if (x == 0) { // At top edge of system
        if (PBC) {
            average = 0.25*(
                R(x, y) +
                R(x, y+stepLength) +
                R(x+halfStepLength, y+halfStepLength) +
                R(systemSize - 1 - halfStepLength, y+halfStepLength));
        } else {
            average = 0.33333333333333333333333*(
                R(x, y) +
                R(x, y+stepLength) +
                R(x+halfStepLength, y+halfStepLength));
        }
    } else { // Inside the system -- nothing to worry about
        average = 0.25*(
            R(x, y) +
            R(x, y+stepLength) +
            R(x+halfStepLength, y+halfStepLength) +
            R(x-halfStepLength, y+halfStepLength));
    }
    R(x, y + halfStepLength) = average + random()*RNGstddv;
}

void DiamondSquare::bottomEdgeDiamonds(const uint x, const uint y, const uint halfStepLength, const double RNGstddv, mat& R) {

    double average = 0.33333333333333333333333*(
        R(x, y) +
        R(x+halfStepLength, y+halfStepLength) +
        R(x+halfStepLength, y-halfStepLength));
    R(x + halfStepLength, y) = average + random()*RNGstddv;
}

void DiamondSquare::rightEdgeDiamonds(const uint x, const uint y, const uint halfStepLength, const double RNGstddv, mat& R) {

    double average = 0.33333333333333333333333*(
        R(x, y) +
        R(x+halfStepLength, y+halfStepLength) +
        R(x-halfStepLength, y+halfStepLength));
    R(x, y + halfStepLength) = average + random()*RNGstddv;
}

inline double DiamondSquare::random() {
    // Returns random number with mean 0
    if (RNG == 0) {
        return 0.0;
    } else if (RNG == 1) {
        return (randu<double>() - 0.5); // uniform distribution in [-0.5,0.5]
    } else if (RNG == 2) {
        return randn<double>(); // standard normal distribution (sigma = 1, my = 0)
    } else {
        return NAN;
    }
}
