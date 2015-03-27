#
# Creates draw1 executable.  'make clean' will rm all object files,
# the executable and a core file.
#
# Style is not what is expected for asn1 (e.g. no header in this
# comment), and this Makefile is not guaranteed to work with your
# code.  Feel free to use it as a template, though. 

draw1a: draw1a.o
	gcc -Wall -std=c99 -o draw1a draw1a.o -lm

draw1a.o: draw1a.c
	gcc -Wall -std=c99 -c draw1a.c -g

clean:
	-rm -f *.o draw1a core
