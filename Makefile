
EXEC := ffdump ffstrings utime memchg

all: $(EXEC)

ffdump: ffdump.c
	gcc -o ffdump ffdump.c -w

ffstrings: ffstrings.c
	gcc -o ffstrings ffstrings.c -w 

ffutime: utime.c
	gcc -o ffutime utime.c 

ffmemchg: memchg
	gcc -o memchg memchg.c

clean:
	rm $(EXEC)
