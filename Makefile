
all: ffdump ffstrings ffutime ffmemchg

ffdump: ffdump.c
	gcc -o ffdump ffdump.c -w

ffstrings: ffstrings.c
	gcc -o ffstrings ffstrings.c -w 

ffutime: utime.c
	gcc -o ffutime utime.c 

ffmemchg: memchg
	gcc -o memchg memchg.c

clean:
	for FILE in $(ls)
	do
		if [ -n "$(file ${FILE} | grep "ELF")" ]
		then
			rm ${FILE}
		fi
	done
