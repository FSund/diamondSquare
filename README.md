diamondSquare
=============

Random terrain generator using the diamond square-algorithm.

Source files best viewed using Qt Creator.

Usage
-----
Compile for use with the Matlab/Octave-script `matlab-compile/compile.m`. Then run in Matlab/Octave using

    main_matlab(power2, H, zMax, zMin, PBC, idum, RNG)

where `power2` decides the size of the system, `H` is a factor that decides how smooth/jagged terrain you want (0.0 is very jagged, 1.0 is smooth), `zMin`/`zMin` sets the max and minimum value of the z-values, `PBC` sets if you want periodic boundaries, `idum` is the seed of the random number generator used, and `RNG` sets the type of random number generator (0 == none, returns 0.0; 1 == uniform; 2 == gaussian). Only `power2` and `H` is need for the program to run.

