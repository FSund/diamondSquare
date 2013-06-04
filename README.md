diamondSquare
=============

Random terrain generator using the diamond square-algorithm.

Source files best viewed using Qt Creator.

Usage
-----
Compile for use with octave with `octave octave-compile/compile.m`. Then run in Octave using 

    main_matlab(seed, power2, H, corners, uniform)

where `seed` is the seed for the random number generator, `power2` decides the size of the system, `H` is a factor that decides how smooth/jagged terrain you want (0.0 is very jagged, 1.0 is smooth), `corners` decides what initial values you want in the corners (set it to 42 to have random corners), and `uniform` sets wether you want uniformly (1) or normally (0) distributed random numbers (should change this to an integer instead of bool...).

