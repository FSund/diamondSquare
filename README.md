diamondSquare
=============

Random terrain generator using the diamond square-algorithm.

Source files best viewed using Qt Creator.

Usage
-----
Compile for use with the Matlab/Octave-script `matlab-compile/compile.m`. Then run in Matlab/Octave using

    main_matlab(power2, H, corner1, corner2, corner3, corner4, sigma, addition, PBC, RNG, seed)

* `power2` decides the size of the system
* `H` is the Hurst-exponent
* corner1-4 is the initial z-value in the corners of the grid
* `sigma` is the inital standard deviation of the random displacement
* `addition` chooses between the successive random addition and successive random displacement algorithm
* `PBC` sets if you want periodic boundaries
* `RNG` sets the type of random number generator (0 == none, returns 0.0; 1 == uniform; 2 == gaussian)
* `seed` is the seed of the random number generator used.

Only `power2` and `H` is need for the program to run, the rest are optional.
