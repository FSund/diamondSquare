#include <armadillo>
#include <mex.h>
#include "diamondSquare.h"

using namespace std;
using namespace arma;

#define R_OUT plhs[0]

void parseArgs(int nArgs, const mxArray* argv[], int &power2, double &H, double &zMin, double &zMax, bool &PBC, long &idum, int &RNG);

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray*prhs[]) {
    int power2;
    double H;
    double zMin;
    double zMax;
    bool PBC;
    long idum;
    int RNG;

    parseArgs(nrhs, prhs, power2, H, zMin, zMax, PBC, idum, RNG);

    // printf("power2 = %d\n", power2);
    // printf("H      = %1.2f\n", H);
    // printf("zMin   = %1.2f\n", zMin);
    // printf("zMax   = %1.2f\n", zMax);
    // printf("PBC    = %d\n", PBC);
    // printf("idum   = %d\n", int(idum));
    // printf("RNG    = %d\n", RNG);

    DiamondSquare generator;
    mat heightMap = generator.generate(power2, H, zMin, zMax, PBC, idum, RNG);

    // create matrix for return argument
    int size = pow(2, power2) + 1;
    R_OUT = mxCreateDoubleMatrix(size, size, mxREAL);
    double *R_out;
    R_out = mxGetPr(R_OUT);

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            R_out[i + j*size] = heightMap(i,j);
        }
    }

    return;
}

void parseArgs(
        int nArgs,
        const mxArray *prhs[],
        int &power2,
        double &H,
        double &zMin,
        double &zMax,
        bool &PBC,
        long &idum,
        int &RNG) {

    if (nArgs < 1) {
        mexErrMsgTxt("Arguments: power2  optional:(H[1,2]  z_min  z_max  PBC[0|1]  idum[unsigned int]  RNG[0|1|2])");
        // exit(1);
    }

    // arguments that are needed
    power2   = int(*mxGetPr(prhs[0]));

    // arguments that have default values
    H         = nArgs > 1 ?       *mxGetPr(prhs[1])   : 1.5;
    zMin      = nArgs > 2 ?       *mxGetPr(prhs[2])   : 0.0;
    zMax      = nArgs > 3 ?       *mxGetPr(prhs[3])   : 1.0;
    PBC       = nArgs > 4 ?  int( *mxGetPr(prhs[4]) ) : true;
    idum      = nArgs > 5 ? long( *mxGetPr(prhs[5]) ) : 1;
    RNG       = nArgs > 6 ?  int( *mxGetPr(prhs[6]) ) : 2;

    if (RNG < 0 || RNG > 2) {
        mexErrMsgTxt("ERROR: Unknown RNG, should be {0,1,2}. Using RNG = 2");
        RNG = 2;
    }
}
