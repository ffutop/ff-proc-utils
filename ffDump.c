#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>

struct map {
    long start_addr;
    long end_addr;
    char op_flag[4];
    long offset;
};

struct options {
    int pid;
    char *output_file;
};
struct options options;

void usage() 
{
    printf("Waiting to Add Help Page!\n");
    exit(0);
}

void dump(char *data, long data_len)
{
    if (options.output_file == NULL)
        return;
    int ofd = open(options.output_file, O_WRONLY | O_APPEND | O_CREAT);
    if (ofd == -1)
    {
        fprintf(stderr, "try to dump mem failed. %s(errno: %d)\n", strerror(errno), errno);
        return;
    }
    ssize_t out_len = write(ofd, data, data_len);
    if (out_len != data_len)
        fprintf(stderr, "try to dump mem failed.. %ld:%ld %s(errno: %d)\n", out_len, data_len, strerror(errno), errno);
    close(ofd);
}

void attach()
{
    if (ptrace(PTRACE_ATTACH, options.pid, NULL, NULL) == -1) 
    {
        fprintf(stderr, "ptract attach failed. %s(errno: %d)\n", strerror(errno), errno);
        exit(0);
    }
    fprintf(stderr, "attach to %d success!\n", options.pid);
    wait(NULL);
}

void peek()
{
    char maps[17];
    sprintf(maps, "/proc/%d/maps", options.pid);
    FILE *fd = fopen(maps, "r");
    if (fd == NULL)
    {
        fprintf(stderr, "open /proc/%d/maps failed. %s(errno: %d)\n", strerror(errno), errno);
        exit(0);
    }

    struct map *map = (struct map *) malloc(sizeof(struct map *));
    
    long word;
    while (fscanf(fd, "%llx-%llx %s %lx %*s %*s%*[^\n]", &map->start_addr, &map->end_addr, map->op_flag, &map->offset) != EOF)
    {
        if (map->op_flag[0] == '-') 
            continue;
        fprintf(stderr, "peek from [%llx-%llx]\n", map->start_addr, map->end_addr);
        long mem_len = map->end_addr - map->start_addr;
        char *data = malloc(mem_len + 1);
        for (long cursor = map->start_addr;cursor < map->end_addr;cursor += sizeof(long))
        {
            if ((word = ptrace(PTRACE_PEEKTEXT, options.pid, cursor, NULL)) == -1 && errno)
            {
                fprintf(stderr, "peek failed. %s(errno: %d)\n", strerror(errno), errno);
                free(data);
                exit(0);
            }
            memcpy(data+cursor-map->start_addr, &word, sizeof(word));
        }
        dump(data, mem_len);
        
        free(data);
    }

    free(map);
}

void detach()
{
    if (ptrace(PTRACE_DETACH, options.pid, NULL, NULL) == -1)
    {
        fprintf("ptract detach failed. %s(errno: %d)\n", strerror(errno), errno);
        exit(0);
    }
    fprintf(stderr, "detach from %d success!", options.pid);
}

int main(int argc, char **argv)
{
    int opt;
    while ((opt = getopt(argc, argv, "ho:p:")) != -1)
    {
        switch(opt)
        {
            case 'h': usage(); break;
            case 'o': 
            {
                size_t len = strlen(optarg);
                options.output_file = malloc(len + 1);
                memcpy(options.output_file, optarg, len);
                break;
            }
            case 'p': options.pid = strtol(optarg, NULL, 10); break;
        }
    }

    attach();
    peek();
    detach();
}
