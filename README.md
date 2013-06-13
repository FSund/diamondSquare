diamondSquare
=============

Random terrain generator using the diamond square-algorithm.

Source files best viewed using Qt Creator.

Usage
-----
Compile for use with octave with `octave octave-compile/compile.m` or with the bash script `compile.sh` (still needs octave installed). Then run in Octave using 

    main_matlab(seed, power2, H, corners, RNG)

where `seed` is the seed for the random number generator, `power2` decides the size of the system, `H` is a factor that decides how smooth/jagged terrain you want (0.0 is very jagged, 1.0 is smooth), `corners` decides what initial values you want in the corners (set it to 42 to have random corners), and `uniform` sets the type of random number generator (0 == none, returns 0.0; 1 == uniform; 2 == gaussian).

