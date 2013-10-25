#include <armadillo>
#include <mex.h>
#include "diamondSquare.h"

using namespace std;
using namespace arma;

#define R_OUT plhs[0]

void parseArgs(int nArgs, const mxArray* argv[], int &power2, double &H, double &zMin, double &zMax, bool &PBC, long &idum, int &RNG);

void mexFunction(int nlhs, mxArray *plhs[], int nArgs, const mxArray* argv[]) {
    int power2;
    double H;
    vec corners = zeros(4);
    double sigma;
    bool addition;
    bool PBC;
    int RNG;
    int seed;

    if (nArgs < 2) {
        mexErrMsgTxt("Arguments: power2  H  optional:( corner(0,0)  corner(1,0)  corner(0,1)  corner(1,1)  sigma  addition  PBC[0|1]  RNG[0|1|2])  seed[unsigned int] )");
        exit(1);
    }

    // arguments that are needed
    power2 = int(*mxGetPr(argv[0]));
    H      = *mxGetPr(argv[1]);

    // argument that have default values
    corners(0) = nArgs > 2  ?     *mxGetPr(argv[2])  : 0.0;
    corners(1) = nArgs > 3  ?     *mxGetPr(argv[3])  : corners(0);
    corners(2) = nArgs > 4  ?     *mxGetPr(argv[4])  : corners(0);
    corners(3) = nArgs > 5  ?     *mxGetPr(argv[5])  : corners(0);
    sigma      = nArgs > 6  ?     *mxGetPr(argv[6])  : 1.0;
    addition   = nArgs > 7  ? int(*mxGetPr(argv[7])) : true;
    PBC        = nArgs > 8  ? int(*mxGetPr(argv[8])) : true;
    RNG        = nArgs > 9  ? int(*mxGetPr(argv[9])) : 2;
    seed       = nArgs > 10 ? ( int(*mxGetPr(argv[10])) >= 0 ? int(*mxGetPr(argv[10])) : 1 ) : 1;

    // printf("power2     = %d\n", power2);
    // printf("H          = %1.2f\n", H);
    // printf("corners(0) = %1.2f\n", corners(0));
    // printf("corners(1) = %1.2f\n", corners(1));
    // printf("corners(2) = %1.2f\n", corners(2));
    // printf("corners(3) = %1.2f\n", corners(3));
    // printf("sigma      = %1.2f\n", sigma);
    // printf("addition   = %d\n", int(addition));
    // printf("PBC        = %d\n", int(PBC));
    // printf("RNG        = %d\n", RNG);
    // printf("seed       = %d\n", seed);

    DiamondSquare generator;
    mat heightMap = generator.generate(power2, H, corners, seed, sigma, addition, PBC, RNG);

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
