#include "diamondSquare.h"

mat &DiamondSquare::generate(
        const uint power2,
        const double H,
        const vec corners,
        const long seed,
        const double sigma,
        const bool addition,
        const bool PBC,
        const uint RNG) {
    this->power2 = power2;
    this->addition = addition;
    this->sigma = sigma;
    this->PBC = PBC;
    this->RNG = RNG;
    systemSize = pow(2.0, power2) + 1;

    srand(seed); // setting the seed of the RNG for both C++'s rand()/srand() and Armadillo's randu()/randn()

    R = zeros<mat>(systemSize, systemSize);
    if (PBC) { // We need the same value in the corners if we are using periodic boundaries
        R(0,0)                       = corners(0);
        R(0,systemSize-1)            = R(0,0);
        R(systemSize-1,0)            = R(0,0);
        R(systemSize-1,systemSize-1) = R(0,0);
    } else {
        R(0,0)                       = corners(0);
        R(0,systemSize-1)            = corners(1);
        R(systemSize-1,0)            = corners(2);
        R(systemSize-1,systemSize-1) = corners(3);
    }

    runDiamondSquare(R, H, sigma);

    return R;
}

void DiamondSquare::runDiamondSquare(mat& R, const double H, const double sigma) {

    double RNGstddv = sigma;
    zerolength = systemSize - 1;
    uint stepLength = zerolength;
    uint halfStepLength = stepLength/2;

    for (uint depth = 1; depth <= power2; depth++) {

        // Squares
        RNGstddv = RNGstddv*pow(0.5, 0.5*H);
        for (uint x = halfStepLength; x <= zerolength - halfStepLength; x += stepLength) {
            for (uint y = halfStepLength; y <= zerolength - halfStepLength; y += stepLength) {
                R(x,y) = square(x, y, halfStepLength, RNGstddv, R);
            }
        }

        // Add random number to the corners of the squares used above
        if (addition) {
            uint limit;
            if (PBC) {
                limit = zerolength - halfStepLength;
            } else {
                limit = zerolength;
            }
            for (uint x = 0; x <= limit; x+=stepLength) {
                for (uint y = 0; y <= limit; y+=stepLength) {
                    R(x,y) = R(x,y) + random()*RNGstddv;
                }
            }
            if (PBC) {
                R(0,zerolength) = R(0,0);
                R(zerolength,0) = R(0,0);
                R(zerolength,zerolength) = R(0,0);
            }
        }

        // Diamonds
        RNGstddv = RNGstddv*pow(0.5, 0.5*H);
        for (uint x = 0; x <= zerolength - halfStepLength; x += stepLength) {
            for (uint y = halfStepLength; y <= zerolength - halfStepLength; y += stepLength) {
                R(x,y) = diamond(x, y, halfStepLength, RNGstddv, R);
            }
        }
        for (uint x = halfStepLength; x <= zerolength - halfStepLength; x += stepLength) {
            for (uint y = 0; y <= zerolength - halfStepLength; y += stepLength) {
                R(x,y) = diamond(x, y, halfStepLength, RNGstddv, R);
            }
        }

        if (PBC) {
            for (uint idx = halfStepLength; idx < zerolength; idx+=halfStepLength) {
                R(idx, zerolength) = R(idx,0);
                R(zerolength, idx) = R(0,idx);
            }
        } else {
            // Bottom edge diamonds
            for (uint y = halfStepLength; y <= zerolength - halfStepLength; y += stepLength) {
                uint x = zerolength;
                R(x,y) = bottomEdgeDiamonds(x, y, halfStepLength, RNGstddv, R);
            }

            // Right edge diamonds
            for (uint x = halfStepLength; x <= zerolength - halfStepLength; x+= stepLength) {
                uint y = zerolength;
                R(x,y) = rightEdgeDiamonds(x, y, halfStepLength, RNGstddv, R);
            }
        }

        if (addition) {
            uint limit;
            if (PBC) {
                limit = zerolength - halfStepLength;
            } else {
                limit = zerolength;
            }
            for (uint x = 0; x <= limit; x+=stepLength) {
                for (uint y = 0; y <= limit; y+=stepLength) {
                    R(x,y) = R(x,y) + random()*RNGstddv;
                }
            }
            for (uint x = halfStepLength; x <= zerolength-halfStepLength; x+=stepLength) {
                for (uint y = halfStepLength; y <= zerolength-halfStepLength; y+=stepLength) {
                    R(x,y) = R(x,y) + random()*RNGstddv;
                }
            }
            if (PBC) {
                R(0,zerolength) = R(0,0);
                R(zerolength,0) = R(0,0);
                R(zerolength,zerolength) = R(0,0);
            }
        }

        stepLength /= 2;
        halfStepLength /= 2;
    }
}

double DiamondSquare::square(
        const uint x,
        const uint y,
        const uint halfStepLength,
        const double RNGstddv,
        const mat &R) {

    return random()*RNGstddv + 0.25*(
        R(x+halfStepLength, y+halfStepLength) +
        R(x+halfStepLength, y-halfStepLength) +
        R(x-halfStepLength, y+halfStepLength) +
        R(x-halfStepLength, y-halfStepLength));
}

double DiamondSquare::diamond(
        const uint x,
        const uint y,
        const uint halfStepLength,
        const double RNGstddv,
        const mat &R) {

    double average;

    if (x == 0) { // At top edge of system
        if (PBC) {
            average = 0.25*(
                R(x, y+halfStepLength) +
                R(x, y-halfStepLength) +
                R(x+halfStepLength, y) +
                R(zerolength-halfStepLength, y));
        } else {
            average = 0.333333333333333333333333333333*(
                R(x, y+halfStepLength) +
                R(x, y-halfStepLength) +
                R(x+halfStepLength, y));
        }
    } else if (y == 0) { // At left edge of system
        if (PBC) {
            average = 0.25*(
                R(x, y+halfStepLength) +
                R(x, zerolength-halfStepLength) +
                R(x+halfStepLength, y) +
                R(x-halfStepLength, y));
        } else {
            average = 0.25*(
                R(x, y+halfStepLength) +
                R(x+halfStepLength, y) +
                R(x-halfStepLength, y));
        }
    } else {
        average = 0.25*(
            R(x, y+halfStepLength) +
            R(x, y-halfStepLength) +
            R(x+halfStepLength, y) +
            R(x-halfStepLength, y));
    }

    return average + random()*RNGstddv;

//    double average;

//    // Point centered at left edge of square
//    if (y == 0) { // At left edge of system
//        if (PBC) {
//            average = 0.25*(
//                R(x, y) +
//                R(x+stepLength, y) +
//                R(x+halfStepLength, y+halfStepLength) +
//                R(x+halfStepLength, systemSize - 1 - halfStepLength));
//        } else {
//            average = 0.33333333333333333333333*(
//                R(x, y) +
//                R(x+stepLength, y) +
//                R(x+halfStepLength, y+halfStepLength));
//        }
//    } else { // Inside the system -- nothing to worry about
//        average = 0.25*(
//            R(x, y) +
//            R(x+stepLength, y) +
//            R(x+halfStepLength, y+halfStepLength) +
//            R(x+halfStepLength, y-halfStepLength));
//    }
//    R(x + halfStepLength, y) = average + random()*RNGstddv;

//    // Point centered at top edge of square
//    if (x == 0) { // At top edge of system
//        if (PBC) {
//            average = 0.25*(
//                R(x, y) +
//                R(x, y+stepLength) +
//                R(x+halfStepLength, y+halfStepLength) +
//                R(systemSize - 1 - halfStepLength, y+halfStepLength));
//        } else {
//            average = 0.33333333333333333333333*(
//                R(x, y) +
//                R(x, y+stepLength) +
//                R(x+halfStepLength, y+halfStepLength));
//        }
//    } else { // Inside the system -- nothing to worry about
//        average = 0.25*(
//            R(x, y) +
//            R(x, y+stepLength) +
//            R(x+halfStepLength, y+halfStepLength) +
//            R(x-halfStepLength, y+halfStepLength));
//    }
//    R(x, y + halfStepLength) = average + random()*RNGstddv;
}

double DiamondSquare::bottomEdgeDiamonds(const uint x, const uint y, const uint halfStepLength, const double RNGstddv, mat& R) {

    return random()*RNGstddv + 0.33333333333333333333333*(
        R(x-halfStepLength, y) +
        R(x, y+halfStepLength) +
        R(x, y-halfStepLength));
}

double DiamondSquare::rightEdgeDiamonds(const uint x, const uint y, const uint halfStepLength, const double RNGstddv, mat& R) {

    return random()*RNGstddv + 0.33333333333333333333333*(
        R(x, y-halfStepLength) +
        R(x+halfStepLength, y) +
        R(x-halfStepLength, y));
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
