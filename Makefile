
all: ffdump ffstrings


ffdump: ffdump.c
	gcc -o ffdump ffdump.c -w

ffstrings: ffstrings.c
	gcc -o ffstrings ffstrings.c -w 

ffutime: utime.c
	gcc -o ffutime utime.c 

clean:
	rm ffstring ffdump
