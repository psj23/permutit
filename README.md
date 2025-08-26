A program that permutes its command-line arguments and writes them to the screen. To generate each new permutation, the permnext() function has to be seeded with the
previous one. It is therefore called nPr times. Since this algorithm allows for one to easily determine the last permutation, it is used in the first call to permnext(). 
The source code is currently the only documentation. It can be messy and the help screen is incomplete...
It's been tested to compile with Microsoft OWindows) and GNU C (Linux) compilers.
For the latter:

gcc -o permutit permutit.c cltail.c combinat.c
