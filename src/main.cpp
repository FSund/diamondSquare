#include <iostream>
#include <armadillo>
#include <src/diamondSquare.h>

using namespace std;
using namespace arma;

int main()
{
    long idum;
    int power2;
    double H;

    idum = -1;
    power2 = 3;
    H = 0.8;

    int L0 = pow(2,power2);
    mat R = zeros(L0+1, L0+1);
    DiamondSquare m(power2, idum);
    R = m.generate(H);

    cout << "R = " << endl << R << endl;

//    R.save("rmat.dat", raw_ascii);

    return 0;
}

// to get QtCreator to run/debug programs correctly:
// $ echo 0 | sudo tee /proc/sys/kernel/yama/ptrace_scope
