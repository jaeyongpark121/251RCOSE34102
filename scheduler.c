#include <stdio.h>
#include <stdlib.h>
#define MAX_PID 100
#define MAX_PROCESS 50

// define process
// used typedef for convenience
typedef struct _process 
{
    int pid;
    int arrival_t;
    int cpu_burst_t;
    int IO_burst_t;
    int IO_request_t;
    int priority;

    int executed_cpu_t;
} process;

// process for empty time
process nothing = {-1,-1,-1,-1,-1,-1,0};

// process list and count
process process_arr[MAX_PROCESS];
int process_count = 0;

// process info
void CheckProcesses();

// setting random PID
int SetRandomPID();

// actually making process 
void AddProcess();

void MakeProcessFast();
void MakeProcessRandomly();

void MakeProcessMenu();


void FCFS()
{
    // start with sorted arr
    process sorted_process_arr[MAX_PROCESS];
    
    for(int i = 0 ; i < process_count; i++)
    {
        sorted_process_arr[i] = process_arr[i];
    }

    // sorting
    for (int i = 0; i < process_count - 1; i++) 
    {
        for (int j = i + 1; j < process_count; j++) 
        {
            if (sorted_process_arr[i].arrival_t > sorted_process_arr[j].arrival_t) 
            {
                process temp = sorted_process_arr[i];
                sorted_process_arr[i] = sorted_process_arr[j];
                sorted_process_arr[j] = temp;
            }
        }
    }

    // printing info
    printf("|      process info     |\n");
    printf("| PID | arrival | burst |\n");
    printf("+=====+=========+=======+\n");
    for(int i = 0; i < process_count; i++)
    {
        printf("|%5d|%9d|%7d|\n",sorted_process_arr[i].pid,sorted_process_arr[i].arrival_t,sorted_process_arr[i].cpu_burst_t);
    }
    printf("+=====+=========+=======+\n");

    // scheduling start
    int completed = 0; // completed processes
    int tick = 0; 
    int Gantt_note[1000][2] = {{0,0},}; // which pid ends when
    int Gantt_index = 0;
    
    printf("| Tick | PID | burst |\n");
    printf("+======+=====+=======+\n");

    process* runningProcess = &nothing;
    while(completed < process_count && tick < 100)
    {
        // from nothing to put arr[0]
        if(sorted_process_arr[0].arrival_t <= tick && runningProcess == &nothing)
        {
            if((*runningProcess).executed_cpu_t > 0)
            {
                (*runningProcess).executed_cpu_t = 0;
                Gantt_note[Gantt_index][0] = (*runningProcess).pid;
                Gantt_note[Gantt_index][1] = tick;
                Gantt_index++;
            }
            runningProcess = &sorted_process_arr[0];
        }

        printf("|%6d|%5d|%7d|\n", tick , (*runningProcess).pid , (*runningProcess).cpu_burst_t);
        tick++;

        //process progress
        (*runningProcess).executed_cpu_t++;
        
        //process done
        if((*runningProcess).executed_cpu_t >= (*runningProcess).cpu_burst_t && (*runningProcess).pid != -1)
        {
            Gantt_note[Gantt_index][0] = (*runningProcess).pid;
            Gantt_note[Gantt_index][1] = tick;
            Gantt_index++;
            runningProcess = &nothing;
            completed++;
            
            // process shift
            for(int i = 0; i < process_count-1; i++)
            {
                sorted_process_arr[i] = sorted_process_arr[i+1];
            }
        }
    }

    // draw Gantt chart
    printf("\n=== Gantt chart ===\n");
    for(int i = 0; i < Gantt_index; i++)
    {
        if(Gantt_note[i][0] == -1) { printf("| EMPTY ~%d |",Gantt_note[i][1]);}
        else { printf("| P[%d] ~%d |",Gantt_note[i][0],Gantt_note[i][1]); }
        
    }
    printf("\n===================\n");

    // calculate turnaround with original arr
    int turnaround_sum = 0;
    int temp_turnaround = 0;
    for(int i = 0; i < process_count; i++)
    {
        for(int j = Gantt_index-1 ; j >= 0; j--)
        {
            if(Gantt_note[j][0] == process_arr[i].pid)
            {
                temp_turnaround = Gantt_note[j][1];
                break;
            }
        }
        turnaround_sum += temp_turnaround - process_arr[i].arrival_t;
        
    }
    
    //evaluation
    printf("\n=== Evaluation ===\n");
    printf("Average turnaround : %f\n", (float)turnaround_sum/process_count);

    int waiting_sum = turnaround_sum;
    for(int i = 0; i < Gantt_index; i++)
    {
        waiting_sum -= process_arr[i].cpu_burst_t;
    }
    printf("Average waiting : %f\n", (float)waiting_sum/process_count);
}

void Non_preemptive_SJF()
{
    // start with sorted arr
    process sorted_process_arr[MAX_PROCESS];
    
    for(int i = 0 ; i < process_count; i++)
    {
        sorted_process_arr[i] = process_arr[i];
    }

    // sorting
    for (int i = 0; i < process_count - 1; i++) 
    {
        for (int j = i + 1; j < process_count; j++) 
        {
            if (sorted_process_arr[i].cpu_burst_t > sorted_process_arr[j].cpu_burst_t) 
            {
                process temp = sorted_process_arr[i];
                sorted_process_arr[i] = sorted_process_arr[j];
                sorted_process_arr[j] = temp;
            }
        }
    }

    // printing info
    printf("|      process info     |\n");
    printf("| PID | arrival | burst |\n");
    printf("+=====+=========+=======+\n");
    for(int i = 0; i < process_count; i++)
    {
        printf("|%5d|%9d|%7d|\n",sorted_process_arr[i].pid,sorted_process_arr[i].arrival_t,sorted_process_arr[i].cpu_burst_t);
    }
    printf("+=====+=========+=======+\n");

    // scheduling start
    int completed = 0; // completed processes
    int tick = 0; 
    int Gantt_note[1000][2] = {{0,0},}; // which pid ends when
    int Gantt_index = 0;
    
    printf("| Tick | PID | burst |\n");
    printf("+======+=====+=======+\n");

    process* runningProcess = &nothing;
    while(completed < process_count && tick < 100)
    {
        // from nothing to put arr[0] ~ arr[n]
        for(int i = 0 ; i < process_count; i++)
        {
            if(sorted_process_arr[i].arrival_t <= tick && runningProcess == &nothing)
            {
                if((*runningProcess).executed_cpu_t > 0)
                {
                    (*runningProcess).executed_cpu_t = 0;
                    Gantt_note[Gantt_index][0] = (*runningProcess).pid;
                    Gantt_note[Gantt_index][1] = tick;
                    Gantt_index++;
                }
                // process shift and select to run
                process temp = sorted_process_arr[i];
                for(int j = i; j > 0; j--)
                {
                    sorted_process_arr[j] = sorted_process_arr[j-1];
                }
                sorted_process_arr[0] = temp; 
                runningProcess = &sorted_process_arr[0];
                break;
            }
        }

        printf("|%6d|%5d|%7d|\n", tick , (*runningProcess).pid , (*runningProcess).cpu_burst_t);
        tick++;

        //process progress
        (*runningProcess).executed_cpu_t++;
        
        //process done
        if((*runningProcess).executed_cpu_t >= (*runningProcess).cpu_burst_t && (*runningProcess).pid != -1)
        {
            Gantt_note[Gantt_index][0] = (*runningProcess).pid;
            Gantt_note[Gantt_index][1] = tick;
            Gantt_index++;
            runningProcess = &nothing;
            completed++;
            
            // process shift
            for(int i = 0; i < process_count-1; i++)
            {
                sorted_process_arr[i] = sorted_process_arr[i+1];
            }
        }
    }

    // draw Gantt chart
    printf("\n=== Gantt chart ===\n");
    for(int i = 0; i < Gantt_index; i++)
    {
        if(Gantt_note[i][0] == -1) { printf("| EMPTY ~%d |",Gantt_note[i][1]);}
        else { printf("| P[%d] ~%d |",Gantt_note[i][0],Gantt_note[i][1]); }
        
    }
    printf("\n===================\n");

    // calculate turnaround with original arr
    int turnaround_sum = 0;
    int temp_turnaround = 0;
    for(int i = 0; i < process_count; i++)
    {
        for(int j = Gantt_index-1 ; j >= 0; j--)
        {
            if(Gantt_note[j][0] == process_arr[i].pid)
            {
                temp_turnaround = Gantt_note[j][1];
                break;
            }
        }
        turnaround_sum += temp_turnaround - process_arr[i].arrival_t;
        
    }
    
    //evaluation
    printf("\n=== Evaluation ===\n");
    printf("Average turnaround : %f\n", (float)turnaround_sum/process_count);

    int waiting_sum = turnaround_sum;
    for(int i = 0; i < Gantt_index; i++)
    {
        waiting_sum -= process_arr[i].cpu_burst_t;
    }
    printf("Average waiting : %f\n", (float)waiting_sum/process_count);
}

void Preemptive_SJF()
{
    // start with sorted arr
    process sorted_process_arr[MAX_PROCESS];
    
    for(int i = 0 ; i < process_count; i++)
    {
        sorted_process_arr[i] = process_arr[i];
    }

    // sorting
    for (int i = 0; i < process_count - 1; i++) 
    {
        for (int j = i + 1; j < process_count; j++) 
        {
            if (sorted_process_arr[i].cpu_burst_t > sorted_process_arr[j].cpu_burst_t) 
            {
                process temp = sorted_process_arr[i];
                sorted_process_arr[i] = sorted_process_arr[j];
                sorted_process_arr[j] = temp;
            }
        }
    }

    // printing info
    printf("|      process info     |\n");
    printf("| PID | arrival | burst |\n");
    printf("+=====+=========+=======+\n");
    for(int i = 0; i < process_count; i++)
    {
        printf("|%5d|%9d|%7d|\n",sorted_process_arr[i].pid,sorted_process_arr[i].arrival_t,sorted_process_arr[i].cpu_burst_t);
    }
    printf("+=====+=========+=======+\n");

    // scheduling start
    int completed = 0; // completed processes
    int tick = 0; 
    int Gantt_note[1000][2] = {{0,0},}; // which pid ends when
    int Gantt_index = 0;
    
    printf("| Tick | PID | burst |\n");
    printf("+======+=====+=======+\n");
    
    // made running process, which saves running process information
    process running = nothing;
    process* runningPtr = &running;
    
    while(completed < process_count && tick < 100)
    {
        // from empty or worse to better
        for(int i = 0 ; i < process_count; i++)
        {
            
            // empty process cannot be preemptive
            if(sorted_process_arr[i].pid == -1) {break;}

            // check empty or better
            if(sorted_process_arr[i].arrival_t <= tick && ((*runningPtr).pid == -1 || 
                sorted_process_arr[i].cpu_burst_t - sorted_process_arr[i].executed_cpu_t <
                (*runningPtr).cpu_burst_t - (*runningPtr).executed_cpu_t))
            {
                //current running was empty
                if((*runningPtr).pid == -1) 
                {

                    // Gantt noting for empty
                    if((*runningPtr).executed_cpu_t > 0)
                    {
                        (*runningPtr).executed_cpu_t = 0;
                        Gantt_note[Gantt_index][0] = (*runningPtr).pid;
                        Gantt_note[Gantt_index][1] = tick;
                        Gantt_index++;
                    }
                    
                    // replacing running
                    running = sorted_process_arr[i];
                    for(int j = i; j < process_count-1 ; j++)
                    {
                        if(sorted_process_arr[j+1].pid != -1) 
                        {
                            sorted_process_arr[j] = sorted_process_arr[j+1];
                            sorted_process_arr[j+1] = nothing;
                        }
                    }
                    runningPtr = &running;
                }
                else
                {
                    // Gantt noting for normal process
                    Gantt_note[Gantt_index][0] = (*runningPtr).pid;
                    Gantt_note[Gantt_index][1] = tick;
                    Gantt_index++;

                    // replacing running and inserting
                    process temp = running;
                    running = sorted_process_arr[i];

                    // deleting
                    for(int j = i; j < process_count-1 ; j++)
                    {
                        if(sorted_process_arr[j+1].pid != -1) 
                        {
                            sorted_process_arr[j] = sorted_process_arr[j+1];
                            sorted_process_arr[j+1] = nothing;
                        }
                    }

                    // temp inserting !!
                    for(int j = process_count-1; j >= 0; j--)
                    {
                        if(sorted_process_arr[j].pid == -1) {continue;}
                        if(temp.cpu_burst_t - temp.executed_cpu_t >
                            sorted_process_arr[j].cpu_burst_t - sorted_process_arr[j].executed_cpu_t)
                        {
                            sorted_process_arr[j+1] = temp;
                            break;
                        }
                        else
                        {
                            sorted_process_arr[j+1] = sorted_process_arr[j];
                            sorted_process_arr[j] = nothing;
                            if(j==0) { sorted_process_arr[j] = temp;}
                        }
                    }
                    runningPtr = &running;
                }
            
                // found first is the best always.
                break;
            }
        }

        printf("|%6d|%5d|%7d|\n", tick , (*runningPtr).pid , (*runningPtr).cpu_burst_t);

        //debug
        for(int i = 0; i < process_count; i++)
        {
            printf("%d ",sorted_process_arr[i].pid);
        }
        printf("\n");

        tick++;

        //process progress
        (*runningPtr).executed_cpu_t++;
        
        //process done
        if((*runningPtr).executed_cpu_t >= (*runningPtr).cpu_burst_t && (*runningPtr).pid != -1)
        {
            Gantt_note[Gantt_index][0] = (*runningPtr).pid;
            Gantt_note[Gantt_index][1] = tick;
            Gantt_index++;
            running = nothing;
            runningPtr = &running;
            completed++;
            
            // process shift.. don't have to do it.
            /*
            for(int i = 0; i < process_count-1; i++)
            {
                sorted_process_arr[i] = sorted_process_arr[i+1];
            }
            */
        }
    }

    // draw Gantt chart
    printf("\n=== Gantt chart ===\n");
    for(int i = 0; i < Gantt_index; i++)
    {
        if(Gantt_note[i][0] == -1) { printf("| EMPTY ~%d |",Gantt_note[i][1]);}
        else { printf("| P[%d] ~%d |",Gantt_note[i][0],Gantt_note[i][1]); }
        
    }
    printf("\n===================\n");

    // calculate turnaround with original arr
    int turnaround_sum = 0;
    int temp_turnaround = 0;
    for(int i = 0; i < process_count; i++)
    {
        for(int j = Gantt_index-1 ; j >= 0; j--)
        {
            if(Gantt_note[j][0] == process_arr[i].pid)
            {
                temp_turnaround = Gantt_note[j][1];
                break;
            }
        }
        turnaround_sum += temp_turnaround - process_arr[i].arrival_t;
        
    }
    
    //evaluation
    printf("\n=== Evaluation ===\n");
    printf("Average turnaround : %f\n", (float)turnaround_sum/process_count);

    int waiting_sum = turnaround_sum;
    for(int i = 0; i < Gantt_index; i++)
    {
        waiting_sum -= process_arr[i].cpu_burst_t;
    }
    printf("Average waiting : %f\n", (float)waiting_sum/process_count);
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
        printf("if you wanna go out, type negative number.\n");
        printf("=================\n");
        printf("please write number to choose > ");
        scanf("%d",&select);

        switch(select)
        {
            case 1:
                MakeProcessMenu();
                break;
            case 2:
                CheckProcesses();
                break;
            case 3:
                if(process_count == 0) { printf("- there are no processes -\n") ;}
                else { FCFS(); }
                break;
            case 4:
                if(process_count == 0) { printf("- there are no processes -\n") ;}
                else { Non_preemptive_SJF(); }
                break;
            case 5:
                if(process_count == 0) { printf("- there are no processes -\n") ;}
                else { Preemptive_SJF(); }
                break;
            case 6:
                if(process_count == 0) { printf("- there are no processes -\n") ;}
                else { ; }
                break;
            case 7:
                if(process_count == 0) { printf("- there are no processes -\n") ;}
                else { ; }
                break;
            default:
                break;
        }
    }
}

void CheckProcesses()
{
    printf("\n=================\n");
    for(int i = 0; i < process_count; i++)
    {
        printf("[%d] arrival : %d, cpu_burst : %d, IO_burst : %d, IO_reqeust : %d, priority: %d\n", 
            process_arr[i].pid,process_arr[i].arrival_t,process_arr[i].cpu_burst_t,process_arr[i].IO_burst_t,process_arr[i].IO_request_t,process_arr[i].priority);
    }
    printf("=================\n");
}

int SetRandomPID()
{
    int _pid_temp = rand()%MAX_PID;
    int pidOverlapped = 0;
    do
    {
        if(pidOverlapped == 1) {_pid_temp = (_pid_temp+1)%MAX_PID;}

        pidOverlapped = 0;
        for(int i = 0; i < process_count; i++)
        {
            if(process_arr[i].pid == _pid_temp)
            {
                pidOverlapped = 1;
            }
        }
    } while (pidOverlapped == 1);

    return _pid_temp;
}

void AddProcess(int _pid, int _arrival, int _cpu_burst, int _IO_burst, int _IO_request, int _priority,int _executed_cpu_t)
{
    process p = {_pid,_arrival,_cpu_burst,_IO_burst,_IO_request,_priority,_executed_cpu_t};
    process_arr[process_count++] = p;
}

void MakeProcessFast()
{
    int _pid = SetRandomPID();
    int _arrival_t = 0;
    int _cpu_burst_t = 0;
    int _IO_burst_t = 0;
    int _IO_request_t = 0;
    int _priority = 0;
    
    int _executed_cpu_t = 0;

    printf("\n=== making process fast ===\n");
    printf("please input arrival time, cpu burst. (max : 20)\n");
    printf("example > 3 3\n");
    printf("=================\n");
    printf("> ");

    int checker = scanf("%d %d",&_arrival_t,&_cpu_burst_t);
    if(checker == 2) 
    {
        if(_arrival_t < 0) {_arrival_t = 0;} else if (_arrival_t > 20) {_arrival_t = 20;}
        if(_cpu_burst_t < 1) {_cpu_burst_t = 1;} else if (_cpu_burst_t > 20) {_cpu_burst_t = 20;}
        
        AddProcess(_pid,_arrival_t,_cpu_burst_t,_IO_burst_t,_IO_request_t,_priority,_executed_cpu_t);
        printf("- process is created -\n");
    }
    else
    {
        printf("- wrong input -\n> ");
        while (getchar() != '\n');
    }
}

void MakeProcessRandomly()
{
    int _pid = SetRandomPID();
    int _arrival_t = 0;
    int _cpu_burst_t = 0;
    int _IO_burst_t = 0;
    int _IO_request_t = 0;
    int _priority = 0;

    int _executed_cpu_t = 0;
    
    _arrival_t = rand()%20;
    _cpu_burst_t = rand()%20 + 1;

    AddProcess(_pid,_arrival_t,_cpu_burst_t,_IO_burst_t,_IO_request_t,_priority,_executed_cpu_t);
    printf("- process is created -\n");
}

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
        printf("if you wanna go out, type negative number.\n");
        printf("=================\n");
        printf("please write number to choose > ");

        scanf("%d",&select);

        switch(select)
        {
            case 1:
                MakeProcessFast();
                break;
            case 2:
                // not yet
                break;
            case 3:
                MakeProcessRandomly();
                break;
            case 4:
                CheckProcesses();
                break;
            default:
                break;
        }
    }
}