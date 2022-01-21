CFLAGS = -g -Wall -pedantic

myrequest: myrequest.c
	gcc $(CFLAGS) -o myrequest.o myrequest.c

.PHONY: clean
clear:
	rm -f *.o
