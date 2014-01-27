#include <iostream>
#include <vector>
#include <cmath>        // pow,
#include <cstdlib>      // atoi, atof, atol
#include <src/diamondSquare/diamondSquare.h>

using namespace std;

//double estimate_hurst_exponent(const vector<vector<double> > &f);

int main(int nArgs, const char *argv[]) {
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

    if (nArgs < 3) {
        cout << "Usage: ./diamondSquare  power2  H  optional:(randomCorners[0|1]  corner(0,0)  corner(1,0)  corner(0,1)  corner(1,1)  initial_RNG_stddv  randomFactor  addition[0|1]  PBC[0|1]  RNG[0|1|2]  seed[unsigned int])" << endl;
        exit(1);
    }

    // arguments that are needed
    power2 = atoi(argv[1]);
    H      = atof(argv[2]);

    // argument that have default values
    randomCorners = nArgs > 3  ? atoi(argv[3])  : true;
    if (!randomCorners) {
        corners.resize(4);
        corners[0] = nArgs > 4  ? atof(argv[4])  : 0.0;
        corners[1] = nArgs > 5  ? atof(argv[5])  : corners[0];
        corners[2] = nArgs > 6  ? atof(argv[6])  : corners[0];
        corners[3] = nArgs > 7  ? atof(argv[7])  : corners[0];
    }

    sigma        = nArgs >  8 ? atof(argv[8])  : 1.0;
    randomFactor = nArgs >  9 ? atof(argv[9])  : 1.0/sqrt(2.0);
    addition     = nArgs > 10 ? atoi(argv[10]) : true;
    PBC          = nArgs > 11 ? atoi(argv[11]) : true;
    RNG          = nArgs > 12 ? atoi(argv[12]) : 2;
    seed         = nArgs > 13 ? atoi(argv[13]) : 1;

    if (!addition && randomFactor != 0.5) {
        cout << "Warning: If not using addition, the random number factor should be 0.5." << endl;
        randomFactor = 0.5;
    }

    cout << "--- Diamond-square settings --------------------" << endl;
    cout << "power2 = " << power2  << endl;
    cout << "H (Hurst exponent) = " << H << endl;
    cout << "corners = "; for (uint i = 0; i < corners.size(); i++) cout << corners[i] << " "; cout << endl;
    cout << "sigma = " << sigma << endl;
    cout << "addition = " << std::boolalpha << addition << std::noboolalpha << endl;
    cout << "PBC = " << std::boolalpha << PBC << std::noboolalpha << endl;
    cout << "RNG = " << RNG << " (0 == no RNG, 1 == uniform, 2 == standard normal distribution)" << endl;
    cout << "seed = " << seed << endl;
    cout << "total number of points in grid = " << pow(pow(2, power2)+1, 2) << endl;
    cout << "------------------------------------------------" << endl;

    DiamondSquare generator(power2, RNG, seed);
    vector<vector<double> > heightMap = generator.generate(H, corners, sigma, randomFactor, addition, PBC);

//    cout << endl << heightMap << endl;

//    estimate_hurst_exponent(heightMap);

    return 0;
}

//double mean(const vector<vector<double> > &f, uint iMin, uint iMax, uint jMin, uint jMax) {
//    double m = 0.0;
//    for (uint i = iMin; i <= iMax; i++) {
//        for (uint j = jMin; j <= jMax; j++) {
//            m += f[i][j];
//        }
//    }
//    m /= (iMax-iMin+1)*(jMax-jMin+1);
//    return m;
//}

//double estimate_hurst_exponent(const vector<vector<double> > &f) {
//    uint nMin = 2;
//    uint nMax = 10;
//    vector<uint> nVec;
//    for (uint i = nMin; i <= nMax; i++) nVec.push_back(i);

//    double theta = 0.0;
//    uint N = f.size();
//    vector<double> sigma_DMA_squared(nVec.size(), 0.0);
//    for (uint k = 0; k < nVec.size(); k++) {
//        uint n = nVec[k];
//        uint mLower = ceil((n-1.0)*(1.0-theta));
//        uint mUpper = -floor((n-1.0)*theta);
//        for (uint i = mLower; i < N+mUpper; i++) {
//            uint iMax = i - mUpper;
//            uint iMin = i - mLower;
//            for (uint j = mLower; j < N+mUpper; j++) {
//                uint jMax = j - mUpper;
//                uint jMin = j - mLower;
//                sigma_DMA_squared[k] += pow(f[i][j] + mean(f, iMin, iMax, jMin, jMax), 2.0);
//            }
//        }
//        sigma_DMA_squared[k] /= pow(N-nMax, 2.0);
//    }
//    return 1.0;
//}

// to get QtCreator to run/debug programs correctly:
// $ echo 0 | sudo tee /proc/sys/kernel/yama/ptrace_scope
