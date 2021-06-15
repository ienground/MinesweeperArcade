all	: main.o
	gcc main.c -lncurses

clean:
	rm a.out *.o