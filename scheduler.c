#include <stdio.h>
#define MAX_PROCESS 50

// define process
// used typedef for convenience
typedef struct _process 
{
    int pid;
    int arrival_time;
    int cpu_burst_time;
    int IO_burst_time;
    int IO_request_time;
    int priority;
} process;

// process list and count
process process_list[MAX_PROCESS];
int process_count = 0;

// check processes
void CheckProcesses()
{
    printf("\n=================\n");
    for(int i = 0; i < process_count; i++)
    {
        printf("[%d] arrival : %d, cpu_burst : %d, IO_burst : %d, IO_reqeust : %d, priority: %d\n", 
                process_list[i].pid,process_list[i].arrival_time,process_list[i].cpu_burst_time,process_list[i].IO_burst_time,process_list[i].IO_request_time,process_list[i].priority);
    }
    printf("=================\n");
}


// actually making process
void AddProcess(int _pid, int _arrival, int _cpu_burst, int _IO_burst, int _IO_request, int _priority)
{
    process p = {_pid,_arrival,_cpu_burst,_IO_burst,_IO_request,_priority};
    process_list[process_count++] = p;
}

// Making process fast
void MakeProcessFast()
{
    int _pid = process_count;
    int _arrival_time = 0;
    int _cpu_burst_time = 0;
    int _IO_burst_time = 0;
    int _IO_request_time = 0;
    int _priority = 0;
    
    printf("\n=== making process fast ===\n");
    printf("please input arrival time, cpu burst. (min : 0, max : 20)\n");
    printf("example > 3 3\n");
    printf("=================\n");
    printf("> ");
    scanf("%d %d",&_arrival_time,&_cpu_burst_time);
    if(_arrival_time < 0) {_arrival_time = 0;} else if (_arrival_time > 20) {_arrival_time = 20;}
    if(_cpu_burst_time < 0) {_cpu_burst_time = 0;} else if (_cpu_burst_time > 20) {_cpu_burst_time = 20;}

    AddProcess(_pid,_arrival_time,_cpu_burst_time,_IO_burst_time,_IO_request_time,_priority);
    printf("- process created -\n");
}

// MakeProcessMenu
void MakeProcessMenu()
{
    int select = 0; // integer for seleciton
    while(select >= 0)
    {
        printf("\n=== making process menu ===\n");
        printf("1. make fast\n");
        printf("2. make slowly with explanation\n");
        printf("3. make randomly\n");
        printf("4. check processes\n");
        printf("=================\n");
        printf("please write number to choose > ");

        scanf("%d",&select);

        switch(select)
        {
            case 1:
                MakeProcessFast();
                break;
            case 2:
                break;
            case 3:
                break;
            case 4:
                CheckProcesses();
                break;
            default:
                break;
        }
    }
}

// main
int main(int argc, char *argv[])
{
    int select = 0; // integer for seleciton
    while(select >= 0)
    {
        printf("\n=== main menu ===\n");
        printf("1. Make new process\n");
        printf("2. Process info\n");
        printf("3. FCFS\n");
        printf("4. Non-Preemptive SJF\n");
        printf("5. Preemptive SJF\n");
        printf("6. Preemptive Priority\n");
        printf("7. Round Robin\n");
        printf("=================\n");
        printf("please write number to choose > ");
        scanf("%d",&select);

        switch(select)
        {
            case 1:
                MakeProcessMenu();
                break;
            default:
                break;
        }
    }
}