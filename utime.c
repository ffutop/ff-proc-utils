#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <utime.h>
#include <sys/types.h>

#define ERR -1
#define OK 0

struct ut {
    char *filename;
    time_t time;
} ut;

int parse_time(char *arg)
{
    struct tm tm;
    memset(&tm, 0, sizeof(tm));
    if (sscanf(arg, " %d-%d-%d %d:%d:%d", &tm.tm_year, &tm.tm_mon, &tm.tm_mday, &tm.tm_hour, &tm.tm_min, &tm.tm_sec) == EOF)
    {
        fprintf(stderr, "parse time failed. format 'yyyy-MM-dd HH:mm:ss'\n");
        return ERR;
    }

    tm.tm_year -= 1900;
    tm.tm_mon -= 1;
    ut.time = mktime(&tm);
    return OK;
}
int parse_args(int argc, char **argv)
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage: utime <filename> <time>\n");
        return ERR;
    }
    
    ut.filename = argv[1];
    return parse_time(argv[2]);
}

int main(int argc, char **argv)
{
    if (parse_args(argc, argv) == ERR)
        return 0;

    struct utimbuf timbuf;
    timbuf.actime = ut.time;
    timbuf.modtime = ut.time;
    utime (ut.filename, &timbuf);
    return 0;
}
