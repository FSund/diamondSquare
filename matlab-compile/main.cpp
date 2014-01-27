#include <vector>
#include <cmath>    // sqrt
#include <mex.h>
#include <src/diamondSquare/diamondSquare.h>

using namespace std;

#define R_OUT plhs[0]

void parseArgs(int nArgs, const mxArray* argv[], int &power2, double &H, double &zMin, double &zMax, bool &PBC, long &idum, int &RNG);

void mexFunction(int nlhs, mxArray *plhs[], int nArgs, const mxArray* argv[]) {
    int power2;
    double H;
    bool randomCorners;
    vector<double> corners; // If DiamondSquare::generate() receives an empty corners vector, it will generate random corners
    double sigma;
    double randomFactor;
    bool addition;
    bool PBC;
    int RNG;
    int seed;

    if (nArgs < 2) {
        mexErrMsgTxt("Arguments: power2  H  optional:(randomCorners[0|1]  corner(0,0)  corner(1,0)  corner(0,1)  corner(1,1)  sigma  randomfactor  addition[0|1]  PBC[0|1]  RNG[0|1|2])  seed[unsigned int])");
        exit(1);
    }

    // arguments that are needed
    power2 = int(*mxGetPr(argv[0]));
    H      = *mxGetPr(argv[1]);

    // argument that have default values
    randomCorners = nArgs > 2 ? int(*mxGetPr(argv[2]))  : true;
    if (!randomCorners) {
        corners.resize(4);
        corners[0] = nArgs > 3 ? *mxGetPr(argv[3]) : 0.0;
        corners[1] = nArgs > 4 ? *mxGetPr(argv[4]) : corners[0];
        corners[2] = nArgs > 5 ? *mxGetPr(argv[5]) : corners[0];
        corners[3] = nArgs > 6 ? *mxGetPr(argv[6]) : corners[0];
    }

    sigma        = nArgs >  7 ?     *mxGetPr(argv[7])   : 1.0;
    randomFactor = nArgs >  8 ?     *mxGetPr(argv[8])   : 1.0/sqrt(2);
    addition     = nArgs >  9 ? int(*mxGetPr(argv[9]))  : true;
    PBC          = nArgs > 10 ? int(*mxGetPr(argv[10])) : true;
    RNG          = nArgs > 11 ? int(*mxGetPr(argv[11])) : 2;
    seed         = nArgs > 12 ? int(*mxGetPr(argv[12])) : 1;

    if (!addition && randomFactor != 0.5) {
        mexWarnMsgTxt("Warning: If not using addition, the random number factor should be 0.5.");
        randomFactor = 0.5;
    }

    mexPrintf("power2        = %d\n", power2);
    mexPrintf("H             = %1.3f\n", H);
    mexPrintf("randomCorners = %d\n", randomCorners);
    for (uint i = 0; i < corners.size(); i++) {mexPrintf("corners(%d)    = %1.2f\n", i, corners[i]);}
    mexPrintf("sigma         = %1.3f\n", sigma);
    mexPrintf("randomFactor  = %1.3f\n", randomFactor);
    mexPrintf("addition      = %d\n", int(addition));
    mexPrintf("PBC           = %d\n", int(PBC));
    mexPrintf("RNG           = %d\n", RNG);
    mexPrintf("seed          = %d\n", seed);

    DiamondSquare generator(power2, RNG, seed);
    vector<vector<double> > heightMap = generator.generate(H, corners, sigma, randomFactor, addition, PBC);

    // create matrix for return argument
    int size = pow(2, power2) + 1;
    R_OUT = mxCreateDoubleMatrix(size, size, mxREAL);
    double *R_out;
    R_out = mxGetPr(R_OUT);

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            R_out[i + j*size] = heightMap[i][j];
        }
    }

    return;
}
