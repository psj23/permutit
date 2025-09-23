When no options are found, this program outputs all permutations of its positional arguments (a positional argument is anything not interpreted as an option or an option-argument), to the standard output device. Redirection is supported in Windows and POSIX-like environments. Only tested with the Microsoft Visual C++ compiler for Windows, and the GNU C compiler for Linux (it'll probably work on anything POSIX with ANSI C). The source code is the documentation.

## Options

**?** : Display informational screen

**c** : Generate only combinations

**i** : Get input from the standard input: Useful for when accepting piped output from another program is desired  
-    The input stream is read line by line using fgets()
-    Any positional arguments that might have been entered at the command prompt are ignored

**r** : Specify a value for r, where **1 <= r <= n**

**s**: Sort positional arguments in ascending ASCII order

Options may appear in any order, and must be grouped before the first positional argument intended for permutaton.

## Compiling

To compile the code using **Windows** and **Visual C++**:

`cl.exe -Fepermutit permutit.c cltail.c combinat.c`

With **Linux** and **GCC**:

`gcc -opermutit permutit.c cltail.c combinat.c`

## Examples

`permutit arg1 arg2 arg3 arg4 arg5`

`permutit -r2 arg1 arg2 arg3`

`permutit -s -r5 arg2 arg3 arg6 arg5 arg1 arg4 arg8 arg7`

`permutit -c -r3 arg1 arg2 arg3 arg4 arg5`
