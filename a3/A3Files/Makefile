#
# Creates a lander executable, and provides a make clean.
#

lander: lander.o memwatch.o
	gcc -Wall -std=c99 -g -o lander lander.o memwatch.o -lm -lcurses

lander.o: lander.c lander.h memwatch.h 
	gcc -std=c99 -Wall -c lander.c  -DMEMWATCH -DMW_STDIO 

memwatch.o: memwatch.c memwatch.h
	gcc -std=c99 -Wall -c memwatch.c -DMEMWATCH -DMW_STDIO -lm -lcurses

clean:
	-rm -f *.o lander core
