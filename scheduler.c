#include <stdio.h>

// define process
// used typedef for convenience
typedef struct _Process 
{
    int pid;
    int arrival_time;
    int cpu_burst_time;
    int IO_burst_time;
    int IO_request_time;
    int priority;
} Process;

int main(int argc, char *argv[])
{
    printf("hello world");
}