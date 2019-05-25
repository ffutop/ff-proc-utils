
all: ffdump ffstrings


ffdump: ffdump.c
	gcc -o ffdump ffdump.c -w

ffstrings: ffstrings.c
	gcc -o ffstrings ffstrings.c -w

clean:
	rm ffstring ffdump
