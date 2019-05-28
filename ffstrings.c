#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>
#include <sys/types.h>

#define PROC_MAPS "/proc/%d/maps"
#define PROC_MEM "/proc/%d/mem"
#define PID_REDRESS 5
#define MAX_FILE_DESC 128
#define HEAP_DESC "[heap]"
#define HEAP_DESC_SIZE 6
#define STACK_DESC "[stack]"
#define STACK_DESC_SIZE 7

struct map {
    long long start_addr;
    long long end_addr;
    char op_flag[4];
    long offset;
    char file_desc[MAX_FILE_DESC];
};

int parse_args(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ffstrings <PID>\n");
        return -1;
    }
    int pid;
    if (sscanf(argv[1], "%d", &pid) == EOF)
    {
        fprintf(stderr, "Usage: ffstrings <PID>\n");
        return -1;
    }
    return pid;
}

#define True 1
#define False 0
int is_heap(char *str)
{
    int len = strlen(str);
    if (strcmp(HEAP_DESC, str+(len-HEAP_DESC_SIZE)) == 0)
        return True;
    return False;
}

int is_stack(char *str)
{
    int len = strlen(str);
    if (strcmp(STACK_DESC, str+(len-STACK_DESC_SIZE)) == 0)
        return True;
    return False;
}

int strings(FILE *mem, unsigned long long start, unsigned long long end, long offset)
{
    if (fseek(mem, start, SEEK_SET) == -1)
    {
        fprintf(stderr, "%s (errno: %d)\n", strerror(errno), errno);
        return -1;
    }
    int isContinue = False;
    for (int count=end-start;count;count--)
    {
        char c = getc(mem);
        if (isprint(c))
        {
            if (isContinue == False)
                putchar('\n');
            putchar(c);
            isContinue = True;
        }
        else
            isContinue = False;
    }
    return 0;
}

int main(int argc, char **argv)
{
    int pid = parse_args(argc, argv);
    if (pid == -1)
        return 0;

    char *procMaps = malloc(strlen(PROC_MAPS) + PID_REDRESS);
    sprintf(procMaps, PROC_MAPS, pid);

    char *procMem = malloc(strlen(PROC_MEM) + PID_REDRESS);
    sprintf(procMem, PROC_MEM, pid);

    FILE *maps = fopen(procMaps, "r");
    FILE *mem = fopen(procMem, "r");
    struct map map;
    while (fscanf(maps, "%llx-%llx %s %lx %*s %*s%[^\n]", &map.start_addr, &map.end_addr, map.op_flag, &map.offset, map.file_desc) != EOF)
        if (is_heap(&map.file_desc) == True || is_stack(&map.file_desc))
            strings(mem, map.start_addr, map.end_addr, map.offset);
    fclose(mem);
    fclose(maps);
    free(procMem);
    free(procMaps);
}
