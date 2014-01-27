#include <src/diamondSquare/diamondSquare.h>

DiamondSquare::DiamondSquare(const uint power2, const uint RNG, const long seed):
    power2(power2),
    systemSize(pow(2.0, power2) + 1),
    R(vector<vector<double> >(systemSize, vector<double>(systemSize))),
    rnd(new Random(-abs(seed))),
    RNG(RNG) {
}

void DiamondSquare::setSeed(const long seed) {
    rnd = new Random(-abs(seed));
}

void DiamondSquare::setRNG(const uint newRNG) {
    RNG = newRNG;
}

vector<vector<double > >&DiamondSquare::generate(
        double H,
        vector<double> corners,
        double sigma,
        double randomFactor,
        bool addition,
        bool PBC) {

    if (corners.size() == 0) { // Use random corners if no values are supplied
        corners.resize(4);
        if (PBC) {
            corners[0] = sigma*random();
            corners[1] = corners[0];
            corners[2] = corners[0];
            corners[3] = corners[0];
        } else {
            corners[0] = sigma*random();
            corners[1] = sigma*random();
            corners[2] = sigma*random();
            corners[3] = sigma*random();
        }
        sigma *= pow(randomFactor, H);
    }

    R[0][0]                       = corners[0];
    R[0][systemSize-1]            = corners[1];
    R[systemSize-1][0]            = corners[2];
    R[systemSize-1][systemSize-1] = corners[3];

    runDiamondSquare(R, H, sigma, randomFactor, addition, PBC);

    return R;
}

// DEBUG //
#include <iomanip>
template <typename T>
std::ostream& operator << (std::ostream& stream, const std::vector<vector<T> >& v) {

    // Usage: 
    //     v = vector<vector<T> >( /*do something smart here*/ );
    //     cout << v;

    ios_base::fmtflags f(stream.flags()); // Store current flags, so we can reset them later
    stream << scientific << setprecision(3); // << fixed // Set formatting flags
    for (typename std::vector<vector<T> >::const_iterator i = v.begin(); i != v.end(); ++i) {
        for (typename std::vector<T>::const_iterator j = i->begin(); j != i->end(); ++j) {
            stream << setw(10) << *j << " ";
        }
        stream << endl;
    }
    stream.flags(f); // Reset flags to previous state
    return stream;
}
// DEBUG //

void DiamondSquare::runDiamondSquare(
        vector<vector<double> >& R,
        const double H,
        double initialSigma,
        const double randomFactor,
        const bool addition,
        const bool _PBC) {

    double sigma = initialSigma;
    uint stepLength = systemSize-1;
    uint halfStepLength = stepLength/2;
    this->PBC = _PBC;

    for (uint depth = 1; depth <= power2; depth++) {

        // Squares //
        for (uint x = halfStepLength; x < systemSize - halfStepLength; x += stepLength) {
            for (uint y = halfStepLength; y < systemSize - halfStepLength; y += stepLength) {
                R[x][y] = meanOfSquare(x, y, halfStepLength, R) + sigma*random();
            }
        }
        if (addition) {
            // Add random number to all old points
            uint limit;
            if (PBC) {
                limit = systemSize - halfStepLength; // Skip right and bottom edge if using periodic boundary conditions
            } else {
                limit = systemSize;
            }
            for (uint x = 0; x < limit; x += stepLength) {
                for (uint y = 0; y < limit; y += stepLength) {
                    R[x][y] += sigma*random();
                }
            }
            if (PBC) {
                // Set corners equal to R[0][0]
                R[0][systemSize-1] = R[0][0];
                R[systemSize-1][0] = R[0][0];
                R[systemSize-1][systemSize-1] = R[0][0];

                // Set bottom edge equal to top edge, and right edge equal to left edge
                for (uint idx = 0; idx < systemSize; idx += stepLength) {
                    R[idx][systemSize-1] = R[idx][0]; // Right/left edge
                    R[systemSize-1][idx] = R[0][idx]; // Bottom/top edge
                }
            }
        }
//        sigma *= pow(0.5, 0.5*H);
        sigma *= pow(randomFactor, H);

        // Diamonds //
        // Every other row of diamond points, starting with the one at (0, halfStepLength)
        for (uint x = 0; x < systemSize - halfStepLength; x += stepLength) {
            for (uint y = halfStepLength; y < systemSize - halfStepLength; y += stepLength) {
                R[x][y] = meanOfDiamond(x, y, halfStepLength, R) + sigma*random();
            }
        }
        // Every other row of diamond points, starting with the one at (halfStepLength, 0)
        for (uint x = halfStepLength; x < systemSize - halfStepLength; x += stepLength) {
            for (uint y = 0; y < systemSize - halfStepLength; y += stepLength) {
                R[x][y] = meanOfDiamond(x, y, halfStepLength, R) + sigma*random();
            }
        }
        if (PBC) {
            // Set bottom edge equal to top, and right edge equal to left
            for (uint idx = halfStepLength; idx < systemSize-1; idx += halfStepLength) {
                R[idx][systemSize-1] = R[idx][0]; // Right/left edge
                R[systemSize-1][idx] = R[0][idx]; // Bottom/top edge
            }
        } else {
            // Bottom edge diamonds
            for (uint y = halfStepLength; y < systemSize - halfStepLength; y += stepLength) {
                uint x = systemSize-1;
                R[x][y] = nonPBCbottomEdgeDiamonds(x, y, halfStepLength, R) + sigma*random();
            }

            // Right edge diamonds
            for (uint x = halfStepLength; x < systemSize - halfStepLength; x+= stepLength) {
                uint y = systemSize-1;
                R[x][y] = nonPBCrightEdgeDiamonds(x, y, halfStepLength, R) + sigma*random();
            }
        }
        if (addition) {
            // Add a random number to all old points
            uint limit;
            if (PBC) {
                limit = systemSize - halfStepLength; // Skip right and bottom edge if using periodic boundary conditions
            } else {
                limit = systemSize;
            }
            for (uint x = 0; x < limit; x += stepLength) {
                for (uint y = 0; y < limit; y += stepLength) {
                    R[x][y] += sigma*random();
                }
            }
            for (uint x = halfStepLength; x < systemSize-halfStepLength; x += stepLength) {
                for (uint y = halfStepLength; y < systemSize-halfStepLength; y += stepLength) {
                    R[x][y] += sigma*random();
                }
            }
            if (PBC) {
                // Set corners equal
                R[0][systemSize-1] = R[0][0];
                R[systemSize-1][0] = R[0][0];
                R[systemSize-1][systemSize-1] = R[0][0];

                // Set bottom edge equal to top, and right edge equal to left
                for (uint idx = halfStepLength; idx < systemSize-1; idx += halfStepLength) {
                    R[idx][systemSize-1] = R[idx][0]; // Right/left edge
                    R[systemSize-1][idx] = R[0][idx]; // Bottom/top edge
                }
            }
        }
//        sigma *= pow(0.5, 0.5*H);
        sigma *= pow(randomFactor, H);

        stepLength /= 2;
        halfStepLength /= 2;
    }
}

double DiamondSquare::meanOfSquare(
        const uint x,
        const uint y,
        const uint halfStepLength,
        const vector<vector<double> >&R) {

    return 0.25*(
        R[x+halfStepLength][y+halfStepLength] +
        R[x+halfStepLength][y-halfStepLength] +
        R[x-halfStepLength][y+halfStepLength] +
        R[x-halfStepLength][y-halfStepLength]);
}

double DiamondSquare::meanOfDiamond(
        const uint x,
        const uint y,
        const uint halfStepLength,
        const vector<vector<double> > &R) {

    double average;

    if (x == 0) { // At top edge of system
        if (PBC) {
            average = 0.25*(
                R[x][y+halfStepLength] +
                R[x][y-halfStepLength] +
                R[x+halfStepLength][y] +
                R[R.size()-1-halfStepLength][y]);
        } else {
            average = (1.0/3.0)*(
                R[x][y+halfStepLength] +
                R[x][y-halfStepLength] +
                R[x+halfStepLength][y]);
        }
    } else if (y == 0) { // At left edge of system
        if (PBC) {
            average = 0.25*(
                R[x][y+halfStepLength] +
                R[x][R[0].size()-1-halfStepLength] +
                R[x+halfStepLength][y] +
                R[x-halfStepLength][y]);
        } else {
            average = (1.0/3.0)*(
                R[x][y+halfStepLength] +
                R[x+halfStepLength][y] +
                R[x-halfStepLength][y]);
        }
    } else {
        average = 0.25*(
            R[x][y+halfStepLength] +
            R[x][y-halfStepLength] +
            R[x+halfStepLength][y] +
            R[x-halfStepLength][y]);
    }

    return average;
}

double DiamondSquare::nonPBCbottomEdgeDiamonds(const uint x, const uint y, const uint halfStepLength, vector<vector<double> >& R) {

    return (1.0/3.0)*(
        R[x-halfStepLength][y] +
        R[x][y+halfStepLength] +
        R[x][y-halfStepLength]);
}

double DiamondSquare::nonPBCrightEdgeDiamonds(const uint x, const uint y, const uint halfStepLength, vector<vector<double> >& R) {

    return (1.0/3.0)*(
        R[x][y-halfStepLength] +
        R[x+halfStepLength][y] +
        R[x-halfStepLength][y]);
}

inline double DiamondSquare::random() {
    // Returns random number with mean 0
    if (RNG == 0) {
        return 0.0;
    } else if (RNG == 1) {
        return rnd->next_double() - 0.5;
    } else if (RNG == 2) {
        return rnd->next_gauss();
    } else {
        return NAN;
    }
}
