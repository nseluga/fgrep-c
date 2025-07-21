all:	fgrep

fgrep:		fgrep.c
	$(CC) -g -Og -Wall -o fgrep fgrep.c

clean:
	rm -f fgrep