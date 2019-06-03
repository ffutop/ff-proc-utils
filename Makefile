
EXEC := ffdump ffstrings utime memchg fftrace

all: $(EXEC)

ffdump: ffdump.c
	gcc -o ffdump ffdump.c -w

ffstrings: ffstrings.c
	gcc -o ffstrings ffstrings.c -w 

ffutime: utime.c
	gcc -o ffutime utime.c 

ffmemchg: memchg.c
	gcc -o memchg memchg.c

fftrace: fftrace.c
	gcc -o fftrace fftrace.c

clean:
	rm $(EXEC)
