#include <iostream>
#include <cstdlib>      // atoi, atof, atol
#include <armadillo>
#include <src/diamondSquare.h>
using namespace std;
using namespace arma;

void parseArgs(int nArgs, const char* argv[], int &power2, double &H, double &zMin, double &zMax, bool &PBC, long &idum, int &RNG);

int main(int nArgs, const char *argv[]) {
    int power2;
    double H;
    double zMin;
    double zMax;
    bool PBC;
    long idum;
    int RNG;

    parseArgs(nArgs, argv, power2, H, zMin, zMax, PBC, idum, RNG);

    cout << "--- Diamond-square settings --------------------" << endl;
    cout << "power2 = " << power2  << endl;
    cout << "H (Hurst exponent) = " << H << endl;
    cout << "z_min = " << zMin << endl;
    cout << "z_max = " << zMax << endl;
    cout << "PBC = " << std::boolalpha << PBC << std::noboolalpha << endl;
    cout << "idum = " << idum << endl;
    cout << "RNG = " << RNG << " (0 == no RNG, 1 == uniform, 2 == standard normal distribution)" << endl;
    cout << "total number of points in grid = " << pow(pow(2, power2)+1, 2) << endl;
    cout << "------------------------------------------------" << endl;

    DiamondSquare generator;
    mat heightMap = generator.generate(power2, H, zMin, zMax, PBC, idum, RNG);

    return 0;
}

void parseArgs(
        int nArgs,
        const char *argv[],
        int &power2,
        double &H,
        double &zMin,
        double &zMax,
        bool &PBC,
        long &idum,
        int &RNG) {

    if (nArgs < 3) {
        cout << "Usage: ./diamondSquare  power2  filename  optional:(H[1,2]  position_of_bottom_surface  position_of_top_surface  surface_delta_z  PBC[0|1]  idum[unsigned int]  RNG[0|1|2])" << endl;
        exit(1);
    }

    // arguments that are needed
    power2   = atoi(argv[1]);

    // argument that have default values
    H         = nArgs > 3 ? atof(argv[2]) : 1.5;
    zMin      = nArgs > 4 ? atof(argv[3]) : 0.0;
    zMax      = nArgs > 5 ? atof(argv[4]) : 1.0;
    PBC       = nArgs > 6 ? atoi(argv[5]) : true;
    idum      = nArgs > 7 ? atol(argv[6]) : 1;
    RNG       = nArgs > 8 ? atoi(argv[7]) : 2;
}

// to get QtCreator to run/debug programs correctly:
// $ echo 0 | sudo tee /proc/sys/kernel/yama/ptrace_scope
