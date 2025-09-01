# permutit
Echoes arguments given at the command-line, in permuted order.
Only tested with the Microsoft Visual C++ compiler for Windows, and the GNU C compiler for Linux.

To compile the code under Windows:

cl.exe -Fepermutit.exe permutit.c cltail.c combinat.c


To compile under Linux:

gcc -opermutit permutit.c cltail.c combinat.c
