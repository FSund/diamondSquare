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

    if (nrhs < 3 || nrhs > 4) {
        mexErrMsgTxt("Three or four input arguments required.");
    } else if (nrhs == 3 || nrhs == 4) {
        idum = long(*mxGetPr(prhs[0]));
        power2 = int(*mxGetPr(prhs[1]));
        H = *mxGetPr(prhs[2]);
        if (nrhs == 4)
            corners = *mxGetPr(prhs[3]);
        else
            corners = 42;
        if(idum < 0) {
            idum = idum;
        } else {
            idum = -idum;
        }
    }

    int L0 = pow(2,power2);
    mat R = zeros(L0+1, L0+1);
    DiamondSquare m(power2, idum);
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

