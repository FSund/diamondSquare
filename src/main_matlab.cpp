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
    bool uniform;

    if (nrhs < 3 || nrhs > 5) {
        mexErrMsgTxt("ERROR: Three, four our five input arguments required.");
    } else {
        idum = long(*mxGetPr(prhs[0]));
        power2 = int(*mxGetPr(prhs[1]));
        H = *mxGetPr(prhs[2]);
        if (nrhs == 3) {
            corners = 42;
            uniform = true;
        } else if (nrhs == 4) {
            corners = *mxGetPr(prhs[3]);
            uniform = true;
        } else if (nrhs == 5) {
            corners = *mxGetPr(prhs[3]);
            uniform = *mxGetPr(prhs[4]);
        } else {
            mexErrMsgTxt("ERROR: Something went wrong!");
        }
    }

    int L0 = pow(2,power2);
    mat R = zeros(L0+1, L0+1);
    DiamondSquare m(power2, idum, uniform);
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

