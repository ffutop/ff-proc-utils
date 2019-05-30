# FangFeng's process operator utils

**ffdump**

dump process stack and heap memory

Usage: `ffdump -o <OUTPUT FILE> -p <PID>`

**ffstrings**
print the strings of printable characters in process stack and heap

Usage: `ffstrings <PID>`

**ffutime**
change files access time and modify time

Usage: `ffutime <FILE NAME> <yyyy-MM-dd HH:mm:ss>`

**memchg**
change process heap memory

Usage: `memchg -p <PID> -f <ORIGIN STR> -t <TARGET STR>`
