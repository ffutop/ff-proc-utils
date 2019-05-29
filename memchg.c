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

#define MAX_MEM_SIZE (1<<28)

struct mdf {
    char *from;
    int from_size;
    char *to;
    int to_size;
    int pid;
} mdf;

struct map {
    long long start_addr;
    long long end_addr;
    char op_flag[4];
    long offset;
    char file_desc[MAX_FILE_DESC];
};

void parse_args(int argc, char **argv)
{
    char opt;
    while ((opt = getopt(argc, argv, "p:f:t:")) != -1)
    {
        switch(opt)
        {
            case 'f': 
                mdf.from = malloc(strlen(optarg) + 1);
                strcpy(mdf.from, optarg);
                mdf.from_size = strlen(mdf.from);
                break;
            case 'p':
                sscanf(optarg, "%d", &mdf.pid);
                break;
            case 't':
                mdf.to = malloc(strlen(optarg) + 1);
                strcpy(mdf.to, optarg);
                mdf.to_size = strlen(mdf.to);
                break;
            default:
                fprintf(stderr, "Unkown opt (%c)\n", opt);
        }
    }
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

int replace(FILE *mem, unsigned long long offset)
{
    if (fseek(mem, offset, SEEK_SET) == -1)
    {
        fprintf(stderr, "[fseek] %s (errno: %d)\n", strerror(errno), errno);
        return -1;
    }

    if (fwrite(mdf.to, mdf.to_size, 1, mem) != 1)
    {
        fprintf(stderr, "[fwrite] %s (errno: %d)\n", strerror(errno), errno);
        return -1;
    }
    return 0;
}

int trace_mem(FILE *mem, unsigned long long start, unsigned long long end, long offset)
{
    int mem_size = end - start - offset;
    if (mem_size > MAX_MEM_SIZE)
    {
        fprintf(stderr, "mem heap is too large\n");
        return -1;
    }
    if (mdf.from == NULL || mdf.to == NULL)
        return 0;
    if (fseek(mem, start+offset, SEEK_SET) == -1)
    {
        fprintf(stderr, "[fseek] %s (errno: %d)\n", strerror(errno), errno);
        return -1;
    }
    char *dump = malloc(mem_size+1);
    if (fread(dump, mem_size, 1, mem) != 1)
    {
        free(dump);
        fprintf(stderr, "[fread] %s (errno: %d)\n", strerror(errno), errno);
        return -1;
    }
    for (int i=0;i<mem_size;i++)
    {
        if (memcmp(mdf.from, dump+i, mdf.from_size) == 0)
        {
            free(dump);
            return replace(mem, start+offset+i);
        }
    }
    free(dump);
    return 0;
}

int main(int argc, char **argv)
{
    parse_args(argc, argv);
    char *procMaps = malloc(strlen(PROC_MAPS) + PID_REDRESS);
    sprintf(procMaps, PROC_MAPS, mdf.pid);

    char *procMem = malloc(strlen(PROC_MEM) + PID_REDRESS);
    sprintf(procMem, PROC_MEM, mdf.pid);

    FILE *maps = fopen(procMaps, "r");
    FILE *mem = fopen(procMem, "r+");
    struct map map;
    while (fscanf(maps, "%llx-%llx %s %lx %*s %*s%[^\n]", &map.start_addr, &map.end_addr, map.op_flag, &map.offset, map.file_desc) != EOF)
        if (is_heap(&map.file_desc[0]) == True)
            trace_mem(mem, map.start_addr, map.end_addr, map.offset);
    fclose(mem);
    fclose(maps);
    free(procMem);
    free(procMaps);
}
