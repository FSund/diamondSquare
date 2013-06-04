#include <armadillo>
#include <mex.h>
#include "diamondSquare.h"

using namespace std;
using namespace arma;

#define R_OUT plhs[0]

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray*prhs[])
{
    long idum;
    int power2;
    double H;
    double corners;
    int RNG;

    if (nrhs < 3 || nrhs > 5) {
        mexErrMsgTxt("ERROR: Three, four our five input arguments required.");
    } else {
        idum = long(*mxGetPr(prhs[0]));
        if (idum == 0)
            mexErrMsgTxt("ERROR: Seed must be non-zero");
        else if (idum > 0)
            idum = -idum;
        power2 = int(*mxGetPr(prhs[1]));
        H = *mxGetPr(prhs[2]);
        if (nrhs == 3) {
            corners = 42;
            RNG = 1;
        } else if (nrhs == 4) {
            corners = *mxGetPr(prhs[3]);
            RNG = 1;
        } else if (nrhs == 5) {
            corners = *mxGetPr(prhs[3]);
            RNG = *mxGetPr(prhs[4]);
            if (RNG > 2 || RNG < 0) {
                mexErrMsgTxt("ERROR: Unknown RNG, should be {0,1,2}.");
            }
        } else {
            mexErrMsgTxt("ERROR: Something went wrong!");
        }
    }

    int L0 = pow(2,power2);
    mat R = zeros(L0+1, L0+1);
    DiamondSquare m(power2, idum, RNG);
    R = m.generate(H, corners);

    // R.fill(-15);

    // create matrix for return argument
    R_OUT = mxCreateDoubleMatrix(L0+1, L0+1, mxREAL);
    double *R_out;
    R_out = mxGetPr(R_OUT);

    for (int i = 0; i < L0+1; i++) {
        for (int j = 0; j < L0+1; j++) {
            R_out[i + j*(L0+1)] = R(i,j);
        }
    }

    return;
}

