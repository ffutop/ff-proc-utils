#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include "fftrace.h"

int trace_fd = -1;
int marker_fd = -1;
char *debugfs = "/sys/kernel/debug";

int ff_trace_on()
{
    char path[256];
    strcpy(path, debugfs);
    strcat(path, "/tracing/tracing_on");
    trace_fd = open(path, O_WRONLY);
    if (trace_fd >= 0)
        write(trace_fd, "1", 1);

    strcpy(path, debugfs);
    strcat(path,"/tracing/trace_marker");
    marker_fd = open(path, O_WRONLY);
    if (marker_fd >= 0)
        write(marker_fd, "In critical area\n", 17);
    return 0;
}

int ff_trace_off()
{
    if (marker_fd >= 0)
	    write(marker_fd, "Out critical area\n", 17);
    if (trace_fd >= 0)
        write(trace_fd, "0", 1);
    close(marker_fd);
    close(trace_fd);
    trace_fd = -1;
    marker_fd = -1;
    return 0;
}
