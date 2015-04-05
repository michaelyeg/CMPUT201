#
# Creates draw1 executable.  'make clean' will rm all object files,
# the executable and a core file.
#
# Style is not what is expected for asn1 (e.g. no header in this
# comment), and this Makefile is not guaranteed to work with your
# code.  Feel free to use it as a template, though. 

draw2: draw2.o memwatch.o
	gcc -Wall -std=c99 -g -o draw2 draw2.o memwatch.o -lm

draw2.o: draw2.c draw2.h memwatch.h
	gcc -Wall -std=c99 -c draw2.c -g -DMEMWATCH -DMW_STDIO

memwatch.o: memwatch.c memwatch.h
	gcc -std=c99 -Wall -c memwatch.c -DMEMWATCH -DMW_STDIO

clean:
	-rm -f *.o draw2 core
