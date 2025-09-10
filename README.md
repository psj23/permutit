# permutit
When no options are found, this program outputs all permutations of its positional arguments (a positional argument is anything not interpreted as an option or an option-argument), to the standard output device. Redirection is supported in Windows and POSIX-like environments. Only tested with the Microsoft Visual C++ compiler for Windows, and the GNU C compiler for Linux (it'll probably work on anything POSIX with ANSI C). The source code is the documentation.

Options:

? : Display a help screen

c : Generate only combinations

i : Get input from the standard input (useful for when accepting piped output from another program is desired; in this case, the input stream is read line by line; any positional arguments that may have been entered at the command prompt are ignored)

r : Specify a value for r, where r is a positive integer less than or equal to the number of positional arguments, n {i.e. 1 <= r <= n}

s: Sort positional arguments in ascending ASCII order

All options, if any at all, must be listed before the first positional argument entered as an object for permutaton. Options may appear in any order.

To compile the code under Windows:

cl.exe -Fepermutit.exe permutit.c cltail.c combinat.c

To compile under Linux:

gcc -opermutit permutit.c cltail.c combinat.c

Examples:

permutit arg1 arg2 arg3 arg4 arg5

permutit -r2 arg1 arg2 arg3

permutit -s -r5 arg2 arg3 arg6 arg5 arg1 arg4 arg8 arg7

permutit -c -r3 arg1 arg2 arg3 arg4 arg5
