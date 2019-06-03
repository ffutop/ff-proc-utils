# FangFeng's process operator utils

## ffdump

dump process stack and heap memory

Usage: `ffdump -o <OUTPUT FILE> -p <PID>`

## ffstrings

print the strings of printable characters in process stack and heap

Usage: `ffstrings <PID>`

## ffutime

change files access time and modify time

Usage: `ffutime <FILE NAME> <yyyy-MM-dd HH:mm:ss>`

## memchg

change process heap memory

Usage: `memchg -p <PID> -f <ORIGIN STR> -t <TARGET STR>`

## fftrace

tracing kernel functions invocation

Usage: 

1. integrate `fftrace.h`

2. update code with `ff_trace_on()` and `ff_trace_off` which you need to trace.

*Hint: you need to insure `/sys/kernel/debug` directory exist. if not, execute `mount -t debugfs nodev /sys/kernel/debug` at first*
