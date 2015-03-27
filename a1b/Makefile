#
# Creates draw1 executable.  'make clean' will rm all object files,
# the executable and a core file.
#
# Style is not what is expected for asn1 (e.g. no header in this
# comment), and this Makefile is not guaranteed to work with your
# code.  Feel free to use it as a template, though. 

draw1b: draw1b.o
	gcc -Wall -std=c99 -o draw1b draw1b.o -lm

draw1b.o: draw1b.c draw1b.h
	gcc -Wall -std=c99 -c draw1b.c -g

clean:
	-rm -f *.o draw1b core
