#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MAX_PID 100
#define MAX_PROCESS 50
#define MAX_TICK 200

// define process
// used typedef for convenience
typedef struct _process 
{
    int pid;
    int arrival_t;
    int cpu_burst_t;

    int IO_burst_t;
    int* IO_request_t;
    int IO_request_count;

    int priority;

    int executed_cpu_t;
} process;

// process for empty time
process nothing = {-1,-1,-1,-1,NULL,-1,-1,0};

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
void DeleteProcess();
void MakeProcessRandomly();

void MakeProcessMenu();

// sorting
void Process_Printing();

void NewFCFS(int eval);
void NewNon_preemptive_SJF(int eval);
void NewPreemptive_SJF(int eval);
void NewNon_preemptive_Priority(int eval);
void NewPreemptive_Priority(int eval);
void NewRound_Robin(int given_quantum, int eval);

// main
int main(int argc, char *argv[])
{
    srand(time(NULL)); // setting random seed
    int select = 0; // integer for selection
    while(select >= 0)
    {
        printf("\n=== main menu ===\n");
        printf("1. make and delete process\n");
        printf("2. Process info\n");
        printf("3. FCFS\n");
        printf("4. Non-Preemptive SJF\n");
        printf("5. Preemptive SJF\n");
        printf("6. Non-Preemptive Priority\n");
        printf("7. Preemptive Priority\n");
        printf("8. Round Robin\n");
        printf("9. Evaluation\n");
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
                else { NewFCFS(0); }
                break;
            case 4:
                if(process_count == 0) { printf("- there are no processes -\n") ;}
                else { NewNon_preemptive_SJF(0); }
                break;
            case 5:
                if(process_count == 0) { printf("- there are no processes -\n") ;}
                else { NewPreemptive_SJF(0); }
                break;
            case 6:
                if(process_count == 0) { printf("- there are no processes -\n") ;}
                else { NewNon_preemptive_Priority(0); }
                break;
            case 7:
                if(process_count == 0) { printf("- there are no processes -\n") ;}
                else { NewPreemptive_Priority(0); }
                break;
            case 8:
                if(process_count == 0) { printf("- there are no processes -\n") ;}
                else 
                {
                    select = 0;
                    printf("\n=================\n");
                    printf("please write time quantum (max = 20) > ");
                    scanf("%d",&select);
                    if(select < 1) {select = 1;} else if (select > 20) {select = 20;}
                    NewRound_Robin(select , 0); 
                }
                break;
            case 9:
                if(process_count == 0) { printf("- there are no processes -\n") ;}
                else 
                {
                    select = 0;
                    printf("\n=================\n");
                    printf("please write time quantum for RR evaluation(max = 20) > ");
                    scanf("%d",&select);
                    if(select < 1) {select = 1;} else if (select > 20) {select = 20;}
                    NewFCFS(1);
                    NewNon_preemptive_SJF(1);
                    NewPreemptive_SJF(1);
                    NewNon_preemptive_Priority(1);
                    NewPreemptive_Priority(1);
                    NewRound_Robin(select , 1);

                }
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
        printf("(idx-%d) [%d] arrival : %d, cpu_burst : %d, IO_burst : %d, priority: %d\n", 
            i,process_arr[i].pid,process_arr[i].arrival_t,process_arr[i].cpu_burst_t,process_arr[i].IO_burst_t,process_arr[i].priority);
        if(process_arr[i].IO_request_count > 0)
        {
            printf("IO request time : ");
            for(int j = 0; j < process_arr[i].IO_request_count; j++)
            {
                printf("%d ",process_arr[i].IO_request_t[j]);
            }
            printf("\n");
        }
    }
    printf("=================\n");
}

int SetRandomPID()
{
    int _pid_temp = rand()%MAX_PID + 1;
    int pidOverlapped = 0;
    do
    {
        if(pidOverlapped == 1) {_pid_temp = (_pid_temp+1)%MAX_PID + 1;}

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

void AddProcess(int _pid, int _arrival, int _cpu_burst, int _IO_burst, int* _IO_request_t, int _IO_request_count, int _priority,int _executed_cpu_t)
{
    int* copied_IO_request_t = NULL;
    if (_IO_request_count > 0 && _IO_request_t != NULL) {
        copied_IO_request_t = (int*)malloc(sizeof(int) * _IO_request_count);
        for (int i = 0; i < _IO_request_count; i++) {
            copied_IO_request_t[i] = _IO_request_t[i];
        }
    }

    process p = {_pid, _arrival, _cpu_burst, _IO_burst, copied_IO_request_t, _IO_request_count, _priority, _executed_cpu_t };

    process_arr[process_count++] = p;
}

void MakeProcessFast()
{
    int _pid = SetRandomPID();
    int _arrival_t = 0;
    int _cpu_burst_t = 0;
    int _IO_burst_t = 0;
    int* _IO_request_t = NULL;
    int _IO_request_count = 0;

    int _priority = 0;
    
    int _executed_cpu_t = 0;

    printf("\n=== making process fast ===\n");
    printf("please input arrival time, cpu burst, priority. (max : 30)\n");
    printf("process with higher priority starts first.\n");
    printf("example > 3 3 3\n");
    printf("=================\n");
    printf("> ");

    int checker = scanf("%d %d %d",&_arrival_t,&_cpu_burst_t,&_priority);
    if(checker == 3) 
    {
        if(_arrival_t < 0) {_arrival_t = 0;} else if (_arrival_t > 30) {_arrival_t = 30;}
        if(_cpu_burst_t < 1) {_cpu_burst_t = 1;} else if (_cpu_burst_t > 30) {_cpu_burst_t = 30;}
        if(_priority < 1) {_priority = 1;} else if (_priority > 30) {_priority = 30;}

        printf("please input IO request burst time and number of IO request time\n");
        printf("number of IO reqeust time should be less than cpu burst time");
        printf("if there's no IO, input 0 0.\n");
        printf("example > 3 3\n");
        printf("=================\n");
        printf("> ");
        
        // IO part
        int checker = scanf("%d %d",&_IO_burst_t, &_IO_request_count);

        if(checker == 2)
        {
            if(_IO_burst_t < 0) {_IO_burst_t = 0;} else if (_IO_burst_t > 30) {_IO_burst_t = 30;}
            if(_IO_request_count < 0) {_IO_request_count = 0;} else if (_IO_request_count >= _cpu_burst_t) { _IO_request_count = _cpu_burst_t-1;}

            if(_IO_burst_t > 0 && _IO_request_count > 0)
            {
            
                // Actually making IO_request_t array
                _IO_request_t = (int*)calloc(_IO_request_count, sizeof(int));

                int count = 0;
                while (count < _IO_request_count)
                {
                    int temp = rand() % (_cpu_burst_t - 1) + 1;
                    int duplicate = 0;

                    for (int j = 0; j < count; j++)
                    {
                        if (_IO_request_t[j] == temp)
                        {
                            duplicate = 1;
                            break;
                        }
                    }

                    if (duplicate == 0)
                    {
                        _IO_request_t[count++] = temp;
                    }
                }
            }
            else { _IO_burst_t = 0; _IO_request_count = 0;} //do not make IO burst

            //sorting IO request time
            for (int i = 0; i < _IO_request_count - 1; i++) 
            {
                for (int j = i + 1; j < _IO_request_count; j++) 
                {
                    if (_IO_request_t[i] > _IO_request_t[j]) 
                    {
                        int temp = _IO_request_t[i];
                        _IO_request_t[i] = _IO_request_t[j];
                        _IO_request_t[j] = temp;
                    }
                }
            }

            AddProcess(_pid,_arrival_t,_cpu_burst_t,_IO_burst_t, _IO_request_t, _IO_request_count, _priority ,_executed_cpu_t);          
            free(_IO_request_t);
            printf("- process is created -\n");
        }
        else
        {
            printf("- wrong input -\n");
            // while (getchar() != '\n');
        } 
    }
    else
    {
        printf("- wrong input -\n");
        // while (getchar() != '\n');
    }
}

void DeleteProcess()
{
    int select = 0;

    CheckProcesses();
    printf("\n=================\n");
    printf("please write index of process to delete > ");
    scanf("%d",&select);

    if(select >= process_count || select < 0)
    {
         printf("- wrong input -\n");
    } 
    else
    {
        if(process_arr[select].pid >= 0)
        {
            free(process_arr[select].IO_request_t);
            for(int j = select; j < process_count; j++)
            {
                process_arr[j] = process_arr[j+1];
            }
            process_count--;
        }
    }
}

void MakeProcessRandomly()
{
    int _pid = SetRandomPID();
    int _arrival_t = rand()%30;
    int _cpu_burst_t = rand()%30 + 1;

    int _IO_burst_t = rand()%30 + 1;
    int* _IO_request_t = NULL;
    int _IO_request_count = 0;

    int _priority = rand()%30 + 1;


    int _executed_cpu_t = 0;

    // setting IO_reqeust
    int temp_io_request_count = rand()%6 ;
    while(temp_io_request_count >= _cpu_burst_t)
    {
        temp_io_request_count--;
    }

    _IO_request_count = temp_io_request_count;

    // actually making IO_request_t array
    if (_IO_request_count > 0 && _IO_burst_t > 0)
    {
        _IO_request_t = (int*)calloc(_IO_request_count, sizeof(int));

        int count = 0;
        while (count < _IO_request_count)
        {
            int temp = rand() % (_cpu_burst_t - 1) + 1;
            int duplicate = 0;

            for (int j = 0; j < count; j++)
            {
                if (_IO_request_t[j] == temp)
                {
                    duplicate = 1;
                    break;
                }
            }

            if (duplicate == 0)
            {
                _IO_request_t[count++] = temp;
            }
        }
    } else {_IO_request_count = 0; _IO_burst_t = 0;}

    //sorting IO_request_array
    for (int i = 0; i < _IO_request_count - 1; i++) 
    {
        for (int j = i + 1; j < _IO_request_count; j++) 
        {
            if (_IO_request_t[i] > _IO_request_t[j]) 
            {
                int temp = _IO_request_t[i];
                _IO_request_t[i] = _IO_request_t[j];
                _IO_request_t[j] = temp;
            }
        }
    }

    AddProcess(_pid,_arrival_t,_cpu_burst_t,_IO_burst_t, _IO_request_t, _IO_request_count, _priority ,_executed_cpu_t);
    free(_IO_request_t);
    printf("- process is created -\n");
}

void MakeProcessMenu()
{
    int select = 0; // integer for seleciton
    while(select >= 0)
    {
        printf("\n=== making process menu ===\n");
        printf("1. make fast\n");
        printf("2. delete with index\n");
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
                DeleteProcess();
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

void Process_Printing(process sorted_process_arr[], int process_count)
{
    printf("|            process info          |\n");
    printf("| PID | arrival | burst | priority |\n");
    printf("+=====+=========+=======+==========+\n");
    for(int i = 0; i < process_count; i++)
    {
        printf("|%5d|%9d|%7d|%10d|\n",sorted_process_arr[i].pid,sorted_process_arr[i].arrival_t,sorted_process_arr[i].cpu_burst_t,sorted_process_arr[i].arrival_t);
    }
    printf("+=====+=========+=======+==========+\n");
}

void NewFCFS(int eval)
{
    process sorted_process_arr[MAX_PROCESS]; // sorted arr prepare
    for(int i = 0 ; i < process_count; i++) { sorted_process_arr[i] = process_arr[i]; }

    process IO_arr[process_count]; // IO array prepare
    for(int i = 0 ; i < process_count; i++) { IO_arr[i] = nothing; }

    int IO_arr_count = 0; // IO array counter

    // sorted arr sorting
    for (int i = 0; i < process_count - 1; i++) 
    {
        for (int j = i + 1; j < process_count; j++) 
        {
            // set front to smaller
            if (sorted_process_arr[i].arrival_t > sorted_process_arr[j].arrival_t) 
            {
                process temp = sorted_process_arr[i];
                sorted_process_arr[i] = sorted_process_arr[j];
                sorted_process_arr[j] = temp;
            }
        }
    }

    if(eval == 0)
    {
        Process_Printing(sorted_process_arr,process_count);
    }

    // preparing variables
    int completed = 0; // completed processes
    int tick = 0; 
    int Gantt_note[1000][3] = {{0,0,0},}; // which pid ends when, and IO
    int Gantt_index = 0;
    process running = nothing; // made running process, which saves running process information

    if (eval == 0)
    {
        printf("| Tick | PID | remaining burst | IO |\n");
        printf("+======+=====+=================+====+\n");
    }
    
    // scheduling start
    while(completed < process_count && tick < MAX_TICK)
    {
        // from empty to first one
        if(sorted_process_arr[0].arrival_t <= tick && running.pid == -1 && sorted_process_arr[0].pid != -1) //empty process cannot be preemptive
        {
            // Gantt noting for empty
            if(running.executed_cpu_t > 0)
            {
                running.executed_cpu_t = 0;
                Gantt_note[Gantt_index][0] = running.pid;
                Gantt_note[Gantt_index][1] = tick;
                Gantt_index++;
            }

            // replace running, dragging the queue
            running = sorted_process_arr[0];
            sorted_process_arr[0] = nothing;
            for(int j = 0; j < process_count-1 ; j++)
            {
                // from i, drag the queue
                if(sorted_process_arr[j+1].pid != -1) 
                {
                    sorted_process_arr[j] = sorted_process_arr[j+1];
                    sorted_process_arr[j+1] = nothing;
                }
            }
        }

        //process && IO progress
        running.executed_cpu_t++;

        for(int i = 0; i < process_count; i++)
        {
            if(IO_arr[i].pid != -1)
            {
                IO_arr[i].IO_burst_t--;

                if(IO_arr[i].IO_burst_t == 0) // IO is end. put ready queue again.
                {
                    // reset IO_burst
                    for (int j = 0; j < process_count; j++)
                    {
                        if(IO_arr[i].pid == process_arr[j].pid)
                        {
                            IO_arr[i].IO_burst_t = process_arr[j].IO_burst_t;
                            break;
                        }
                    }

                    // insert. find proper position starting from right.
                    for(int j = process_count-1; j >= 0; j--)
                    {
                        if(sorted_process_arr[j].pid == -1) 
                        {
                            if(j == 0) { sorted_process_arr[j] = IO_arr[i];}
                            continue;
                        }
                        else
                        {
                            sorted_process_arr[j+1] = IO_arr[i];
                            break;
                        }
                    }

                    IO_arr[i] = nothing;
                }
            }
        }

        // check IO time or not
        int IO = 0;
        for(int i = 0; i < running.IO_request_count; i++)
        {
            if(running.executed_cpu_t == running.IO_request_t[i]) // it's IO time.
            {
                IO = 1;
                break;
            }
            else if(running.executed_cpu_t < running.IO_request_t[i]) { break; }
        }

        if(eval == 0)
        {
            if (running.pid == -1) { printf("|%6d|%5d|%17d|    |\n", tick, running.pid, running.cpu_burst_t);}
            else if (IO > 0) { printf("|%6d|%5d|%17d| IO |\n", tick, running.pid, running.cpu_burst_t - running.executed_cpu_t); }
            else { printf("|%6d|%5d|%17d|    |\n", tick, running.pid, running.cpu_burst_t - running.executed_cpu_t); }
        }
        //progress tick
        tick++;
        
        //check process done
        if(running.executed_cpu_t >= running.cpu_burst_t && running.pid != -1)
        {
            Gantt_note[Gantt_index][0] = running.pid;
            Gantt_note[Gantt_index][1] = tick;
            Gantt_index++;

            running = nothing;
            completed++;
        }
        
        //if IO time then put it into a queue
        if(IO > 0)
        {
            IO = 0;

            Gantt_note[Gantt_index][0] = running.pid;
            Gantt_note[Gantt_index][1] = tick;
            Gantt_note[Gantt_index][2] = 1;
            Gantt_index++;
            
            for(int i = 0; i < process_count; i++)
            {
                if(IO_arr[i].pid == -1)
                {
                    IO_arr[i] = running;
                    break;
                }
            }
            running = nothing;
        }

        //debug
        /*
        printf("ready queue : ");
        for(int i = 0; i < process_count; i++)
        {
            printf("%d ",sorted_process_arr[i].pid);
        }
        printf("\n");

        printf("IO queue : ");
        for(int i = 0; i < process_count; i++)
        {
            printf("%d ",IO_arr[i].pid);
        }
        printf("\n");
        */

    } 
    // scheduling finished

    // draw Gantt chart
    if(eval == 0)
    {
        printf("\n=== Gantt chart ===\n");
        for(int i = 0; i < Gantt_index; i++)
        {
            if(Gantt_note[i][0] == -1) { printf("| EMPTY ~%d |",Gantt_note[i][1]);}
            else
            {
                printf("| P[%d] ~%d",Gantt_note[i][0],Gantt_note[i][1]); 
                if(Gantt_note[i][2] == 0) {printf(" |");}
                else if(Gantt_note[i][2] == 1) {printf("(IO) |");}
            }   
        }
        printf("\n===================\n");
    }

    //evaluation
    if(eval == 0) { printf("\n=== Evaluation ===\n"); }
    else { printf("\n[ FCFS ] "); }

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
        turnaround_sum += (temp_turnaround - process_arr[i].arrival_t);
        
    }
    printf("Average turnaround : %f | ", (float)turnaround_sum/process_count);

    int waiting_sum = turnaround_sum;
    for(int i = 0; i < process_count; i++)
    {
        waiting_sum -= process_arr[i].cpu_burst_t;
        waiting_sum -= (process_arr[i].IO_burst_t * process_arr[i].IO_request_count);
    }
    printf("Average waiting : %f\n", (float)waiting_sum/process_count);
}

void NewNon_preemptive_SJF(int eval)
{
    process sorted_process_arr[MAX_PROCESS]; // sorted arr prepare
    for(int i = 0 ; i < process_count; i++) { sorted_process_arr[i] = process_arr[i]; }

    process IO_arr[process_count]; // IO array prepare
    for(int i = 0 ; i < process_count; i++) { IO_arr[i] = nothing; }

    int IO_arr_count = 0; // IO array counter

    // sorting
    for (int i = 0; i < process_count - 1; i++) 
    {
        for (int j = i + 1; j < process_count; j++) 
        {
            // set front to smaller
            if (sorted_process_arr[i].cpu_burst_t > sorted_process_arr[j].cpu_burst_t) 
            {
                process temp = sorted_process_arr[i];
                sorted_process_arr[i] = sorted_process_arr[j];
                sorted_process_arr[j] = temp;
            }
        }
    }
    
    if(eval == 0)
    {
        Process_Printing(sorted_process_arr,process_count);
    }

    // preparing variables
    int completed = 0; // completed processes
    int tick = 0; 
    int Gantt_note[1000][3] = {{0,0,0},}; // which pid ends when, and IO
    int Gantt_index = 0;
    process running = nothing; // made running process, which saves running process information
    
    if (eval == 0)
    {
        printf("| Tick | PID | remaining burst | IO |\n");
        printf("+======+=====+=================+====+\n");
    }
    
    // scheduling start
    while(completed < process_count && tick < MAX_TICK)
    {
        // from empty to better
        // check from ahead of the queue.
        for(int i = 0 ; i < process_count; i++)
        {
            // check empty 
            if(sorted_process_arr[i].arrival_t <= tick && (running.pid == -1) && sorted_process_arr[i].pid != -1)
            {
                // Gantt noting for empty
                if(running.executed_cpu_t > 0)
                {
                    running.executed_cpu_t = 0;
                    Gantt_note[Gantt_index][0] = running.pid;
                    Gantt_note[Gantt_index][1] = tick;
                    Gantt_index++;
                }
                
                // replace running, reorganize queue
                running = sorted_process_arr[i];
                sorted_process_arr[i] = nothing;
                for(int j = i; j < process_count-1 ; j++)
                {
                    // from i, drag the queue
                    if(sorted_process_arr[j+1].pid != -1) 
                    {
                        sorted_process_arr[j] = sorted_process_arr[j+1];
                        sorted_process_arr[j+1] = nothing;
                    }
                }
                // found first is the best always.
                break;
            }
        }
       
        //process && IO progress
        running.executed_cpu_t++;
        
        for(int i = 0; i < process_count; i++)
        {
            if(IO_arr[i].pid != -1)
            {
                IO_arr[i].IO_burst_t--;

                if(IO_arr[i].IO_burst_t == 0) // IO is end. put ready queue again.
                {
                    // reset IO_burst
                    for (int j = 0; j < process_count; j++)
                    {
                        if(IO_arr[i].pid == process_arr[j].pid)
                        {
                            IO_arr[i].IO_burst_t = process_arr[j].IO_burst_t;
                            break;
                        }
                    }

                    // insert. find proper position starting from right.
                    for(int j = process_count-1; j >= 0; j--)
                    {
                        if(sorted_process_arr[j].pid == -1) 
                        {
                            if(j == 0) { sorted_process_arr[j] = IO_arr[i];}
                            continue;
                        }
                        
                        if(IO_arr[i].cpu_burst_t - IO_arr[i].executed_cpu_t
                            > sorted_process_arr[j].cpu_burst_t - sorted_process_arr[j].executed_cpu_t)
                        {
                            sorted_process_arr[j+1] = IO_arr[i];
                            break;
                        }
                        else
                        {
                            sorted_process_arr[j+1] = sorted_process_arr[j];
                            sorted_process_arr[j] = nothing;
                            if(j==0) { sorted_process_arr[j] = IO_arr[i];}
                        }
                    }

                    IO_arr[i] = nothing;
                }
            }
        }

        // check if IO time or not
        int IO = 0;
        for(int i = 0; i < running.IO_request_count; i++)
        {
            if(running.executed_cpu_t == running.IO_request_t[i]) // it's IO time.
            {
                IO = 1;
                break;
            }
            else if(running.executed_cpu_t < running.IO_request_t[i]) { break; }
        }

        if(eval == 0)
        {
            if (running.pid == -1) { printf("|%6d|%5d|%17d|    |\n", tick, running.pid, running.cpu_burst_t);}
            else if (IO > 0) { printf("|%6d|%5d|%17d| IO |\n", tick, running.pid, running.cpu_burst_t - running.executed_cpu_t); }
            else { printf("|%6d|%5d|%17d|    |\n", tick, running.pid, running.cpu_burst_t - running.executed_cpu_t); }
        }
        
        //progress tick
        tick++;
        
        //check process done
        if(running.executed_cpu_t >= running.cpu_burst_t && running.pid != -1)
        {
            Gantt_note[Gantt_index][0] = running.pid;
            Gantt_note[Gantt_index][1] = tick;
            Gantt_index++;

            running = nothing;
            completed++;
        }
        
        //if IO then switch
        if(IO > 0)
        {
            IO = 0;

            Gantt_note[Gantt_index][0] = running.pid;
            Gantt_note[Gantt_index][1] = tick;
            Gantt_note[Gantt_index][2] = 1;
            Gantt_index++;
            
            for(int i = 0; i < process_count; i++)
            {
                if(IO_arr[i].pid == -1)
                {
                    IO_arr[i] = running;
                    break;
                }
            }
            running = nothing;
        }

        //debug
        /*
        printf("ready queue : ");
        for(int i = 0; i < process_count; i++)
        {
            printf("%d ",sorted_process_arr[i].pid);
        }
        printf("\n");

        printf("IO queue : ");
        for(int i = 0; i < process_count; i++)
        {
            printf("%d ",IO_arr[i].pid);
        }
        printf("\n");
        */
       
    } // scheduling finished

    // draw Gantt chart
    if(eval == 0)
    {
        printf("\n=== Gantt chart ===\n");
        for(int i = 0; i < Gantt_index; i++)
        {
            if(Gantt_note[i][0] == -1) { printf("| EMPTY ~%d |",Gantt_note[i][1]);}
            else
            {
                printf("| P[%d] ~%d",Gantt_note[i][0],Gantt_note[i][1]); 
                if(Gantt_note[i][2] == 0) {printf(" |");}
                else if(Gantt_note[i][2] == 1) {printf("(IO) |");}
            }   
        }
        printf("\n===================\n");
    }
    
    //evaluation
    if(eval == 0) { printf("\n=== Evaluation ===\n"); }
    else { printf("\n[ Nonpreemptive SJF ] "); }
    
    // calculate turnaround with original arr
    int turnaround_sum = 0;
    int temp_turnaround = 0;
    for (int i = 0; i < process_count; i++)
    {
        for (int j = Gantt_index - 1; j >= 0; j--)
        {
            if (Gantt_note[j][0] == process_arr[i].pid)
            {
                temp_turnaround = Gantt_note[j][1];
                break;
            }
        }
        turnaround_sum += (temp_turnaround - process_arr[i].arrival_t);

    }
    printf("Average turnaround : %f | ", (float)turnaround_sum / process_count);

    int waiting_sum = turnaround_sum;
    for (int i = 0; i < process_count; i++)
    {
        waiting_sum -= process_arr[i].cpu_burst_t;
        waiting_sum -= (process_arr[i].IO_burst_t * process_arr[i].IO_request_count);
    }
    printf("Average waiting : %f\n", (float)waiting_sum / process_count);
}

void NewPreemptive_SJF(int eval)
{
    process sorted_process_arr[MAX_PROCESS]; // sorted arr prepare
    for (int i = 0; i < process_count; i++) { sorted_process_arr[i] = process_arr[i]; }

    process IO_arr[process_count]; // IO array prepare
    for (int i = 0; i < process_count; i++) { IO_arr[i] = nothing; }

    int IO_arr_count = 0; // IO array counter

    // sorting
    for (int i = 0; i < process_count - 1; i++)
    {
        for (int j = i + 1; j < process_count; j++)
        {
            // set front to smaller
            if (sorted_process_arr[i].cpu_burst_t > sorted_process_arr[j].cpu_burst_t)
            {
                process temp = sorted_process_arr[i];
                sorted_process_arr[i] = sorted_process_arr[j];
                sorted_process_arr[j] = temp;
            }
        }
    }

    // printing info
    if(eval == 0)
    {
        Process_Printing(sorted_process_arr,process_count);
    }   

    // preparing variables
    int completed = 0; // completed processes
    int tick = 0; 
    int Gantt_note[1000][3] = { { 0,0,0},}; // which pid ends when, and IO
    int Gantt_index = 0;
    process running = nothing; // made running process, which saves running process information
    
    if(eval == 0)
    {
        printf("| Tick | PID | remaining burst | IO |\n");
        printf("+======+=====+=================+====+\n");
    }
    
    while(completed < process_count && tick < MAX_TICK)
    {
        // from empty or worse to better
        for(int i = 0 ; i < process_count; i++)
        {
            // empty process cannot be preemptive
            if(sorted_process_arr[i].pid == -1) {break;}

            // check empty or better
            if(sorted_process_arr[i].arrival_t <= tick && (running.pid == -1 || 
                sorted_process_arr[i].cpu_burst_t - sorted_process_arr[i].executed_cpu_t <
                running.cpu_burst_t - running.executed_cpu_t))
            {
                //current running was empty
                if(running.pid == -1) 
                {

                    // Gantt noting for empty
                    if(running.executed_cpu_t > 0)
                    {
                        running.executed_cpu_t = 0;
                        Gantt_note[Gantt_index][0] = running.pid;
                        Gantt_note[Gantt_index][1] = tick;
                        Gantt_index++;
                    }
                    
                    // replacing running
                    running = sorted_process_arr[i];
                    sorted_process_arr[i] = nothing;
                    for(int j = i; j < process_count-1 ; j++)
                    {
                        if(sorted_process_arr[j+1].pid != -1) 
                        {
                            sorted_process_arr[j] = sorted_process_arr[j+1];
                            sorted_process_arr[j+1] = nothing;
                        }
                    }
                }
                else //running normal process
                {
                    // Gantt noting for normal process
                    Gantt_note[Gantt_index][0] = running.pid;
                    Gantt_note[Gantt_index][1] = tick;
                    Gantt_index++;

                    // save current running to put again
                    process temp = running;

                    //replace running, reorganize queue
                    running = sorted_process_arr[i];
                    sorted_process_arr[i] = nothing;
                    for(int j = i; j < process_count-1 ; j++)
                    {
                        //from i, drag the queue
                        if(sorted_process_arr[j+1].pid != -1) 
                        {
                            sorted_process_arr[j] = sorted_process_arr[j+1];
                            sorted_process_arr[j+1] = nothing;
                        }
                    }

                    // insert temp, find proper position starting from right
                    for(int j = process_count-1; j >= 0; j--)
                    {
                        if(sorted_process_arr[j].pid == -1)
                        {
                            if(j == 0) { sorted_process_arr[j] = temp;}
                            continue;
                        }
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
                }
            
                // found first is the best always.
                break;
            }
        }

        //process & IO Progress
        running.executed_cpu_t++;

        for (int i = 0; i < process_count; i++)
        {
            if (IO_arr[i].pid != -1)
            {
                IO_arr[i].IO_burst_t--;

                if (IO_arr[i].IO_burst_t == 0) // IO is end. put ready queue again.
                {
                    // reset IO_burst
                    for (int j = 0; j < process_count; j++)
                    {
                        if (IO_arr[i].pid == process_arr[j].pid)
                        {
                            IO_arr[i].IO_burst_t = process_arr[j].IO_burst_t;
                            break;
                        }
                    }

                    // insert. find proper position starting from right.
                    for (int j = process_count - 1; j >= 0; j--)
                    {
                        if (sorted_process_arr[j].pid == -1)
                        {
                            if (j == 0) { sorted_process_arr[j] = IO_arr[i]; }
                            continue;
                        }

                        if (IO_arr[i].cpu_burst_t - IO_arr[i].executed_cpu_t
                            > sorted_process_arr[j].cpu_burst_t - sorted_process_arr[j].executed_cpu_t)
                        {
                            sorted_process_arr[j + 1] = IO_arr[i];
                            break;
                        }
                        else
                        {
                            sorted_process_arr[j + 1] = sorted_process_arr[j];
                            sorted_process_arr[j] = nothing;
                            if (j == 0) { sorted_process_arr[j] = IO_arr[i]; }
                        }
                    }

                    IO_arr[i] = nothing;
                }
            }
        }

        // check if IO time or not
        int IO = 0;
        for (int i = 0; i < running.IO_request_count; i++)
        {
            if (running.executed_cpu_t == running.IO_request_t[i]) // it's IO time.
            {
                IO = 1;
                break;
            }
            else if (running.executed_cpu_t < running.IO_request_t[i]) { break; }
        }

        if(eval == 0)
        {
            if (running.pid == -1) { printf("|%6d|%5d|%17d|    |\n", tick, running.pid, running.cpu_burst_t);}
            else if (IO > 0) { printf("|%6d|%5d|%17d| IO |\n", tick, running.pid, running.cpu_burst_t - running.executed_cpu_t); }
            else { printf("|%6d|%5d|%17d|    |\n", tick, running.pid, running.cpu_burst_t - running.executed_cpu_t); }
        }

        //progress tick
        tick++;
        
        //process done
        if(running.executed_cpu_t >= running.cpu_burst_t && running.pid != -1)
        {
            Gantt_note[Gantt_index][0] = running.pid;
            Gantt_note[Gantt_index][1] = tick;
            Gantt_index++;

            running = nothing;
            completed++;
        }

        //if IO then switch
        if (IO > 0)
        {
            IO = 0;

            Gantt_note[Gantt_index][0] = running.pid;
            Gantt_note[Gantt_index][1] = tick;
            Gantt_note[Gantt_index][2] = 1;
            Gantt_index++;

            for (int i = 0; i < process_count; i++)
            {
                if (IO_arr[i].pid == -1)
                {
                    IO_arr[i] = running;
                    break;
                }
            }
            running = nothing;
        }
        //debug
        /*
        printf("ready queue : ");
        for(int i = 0; i < process_count; i++)
        {
            printf("%d ",sorted_process_arr[i].pid);
        }
        printf("\n");

        printf("IO queue : ");
        for(int i = 0; i < process_count; i++)
        {
            printf("%d ",IO_arr[i].pid);
        }
        printf("\n");
        */
    } // scheduling finished

    // draw Gantt chart
    if(eval == 0)
    {
        printf("\n=== Gantt chart ===\n");
        for (int i = 0; i < Gantt_index; i++)
        {
            if (Gantt_note[i][0] == -1) { printf("| EMPTY ~%d |", Gantt_note[i][1]); }
            else
            {
                printf("| P[%d] ~%d", Gantt_note[i][0], Gantt_note[i][1]);
                if (Gantt_note[i][2] == 0) { printf(" |"); }
                else if (Gantt_note[i][2] == 1) { printf("(IO) |"); }
            }
        }
        printf("\n===================\n");
    }

    //evaluation
    if(eval == 0) { printf("\n=== Evaluation ===\n"); }
    else { printf("\n[ Preemptive SJF ] "); }

    // calculate turnaround with original arr
    int turnaround_sum = 0;
    int temp_turnaround = 0;
    for (int i = 0; i < process_count; i++)
    {
        for (int j = Gantt_index - 1; j >= 0; j--)
        {
            if (Gantt_note[j][0] == process_arr[i].pid)
            {
                temp_turnaround = Gantt_note[j][1];
                break;
            }
        }
        turnaround_sum += (temp_turnaround - process_arr[i].arrival_t);

    }
    printf("Average turnaround : %f | ", (float)turnaround_sum / process_count);

    int waiting_sum = turnaround_sum;
    for (int i = 0; i < process_count; i++)
    {
        waiting_sum -= process_arr[i].cpu_burst_t;
        for (int j = 0; j < process_arr[i].IO_request_count; j++)
        {
            // IO burst * count!
            waiting_sum -= process_arr[i].IO_burst_t;
        }
    }
    printf("Average waiting : %f\n", (float)waiting_sum / process_count);
}

void NewNon_preemptive_Priority(int eval)
{
    process sorted_process_arr[MAX_PROCESS]; // sorted arr prepare
    for(int i = 0 ; i < process_count; i++) { sorted_process_arr[i] = process_arr[i]; }

    process IO_arr[process_count]; // IO array prepare
    for(int i = 0 ; i < process_count; i++) { IO_arr[i] = nothing; }

    int IO_arr_count = 0; // IO array counter

    // sorting
    for (int i = 0; i < process_count - 1; i++) 
    {
        for (int j = i + 1; j < process_count; j++) 
        {
            // set front to bigger
            if (sorted_process_arr[i].priority < sorted_process_arr[j].priority) 
            {
                process temp = sorted_process_arr[i];
                sorted_process_arr[i] = sorted_process_arr[j];
                sorted_process_arr[j] = temp;
            }
        }
    }
    
    if(eval == 0)
    {
        Process_Printing(sorted_process_arr,process_count);
    }

    // preparing variables
    int completed = 0; // completed processes
    int tick = 0; 
    int Gantt_note[1000][3] = {{0,0,0},}; // which pid ends when, and IO
    int Gantt_index = 0;
    process running = nothing; // made running process, which saves running process information
    
    if (eval == 0)
    {
        printf("| Tick | PID | remaining burst | IO |\n");
        printf("+======+=====+=================+====+\n");
    }
    
    // scheduling start
    while(completed < process_count && tick < MAX_TICK)
    {
        // from empty to better
        // check from ahead of the queue.
        for(int i = 0 ; i < process_count; i++)
        {
            // check empty 
            if(sorted_process_arr[i].arrival_t <= tick && (running.pid == -1) && sorted_process_arr[i].pid != -1)
            {
                // Gantt noting for empty
                if(running.executed_cpu_t > 0)
                {
                    running.executed_cpu_t = 0;
                    Gantt_note[Gantt_index][0] = running.pid;
                    Gantt_note[Gantt_index][1] = tick;
                    Gantt_index++;
                }
                
                // replace running, reorganize queue
                running = sorted_process_arr[i];
                sorted_process_arr[i] = nothing;
                for(int j = i; j < process_count-1 ; j++)
                {
                    // from i, drag the queue
                    if(sorted_process_arr[j+1].pid != -1) 
                    {
                        sorted_process_arr[j] = sorted_process_arr[j+1];
                        sorted_process_arr[j+1] = nothing;
                    }
                }
                // found first is the best always.
                break;
            }
        }
       
        //process && IO progress
        running.executed_cpu_t++;
        
        for(int i = 0; i < process_count; i++)
        {
            if(IO_arr[i].pid != -1)
            {
                IO_arr[i].IO_burst_t--;

                if(IO_arr[i].IO_burst_t == 0) // IO is end. put ready queue again.
                {
                    // reset IO_burst
                    for (int j = 0; j < process_count; j++)
                    {
                        if(IO_arr[i].pid == process_arr[j].pid)
                        {
                            IO_arr[i].IO_burst_t = process_arr[j].IO_burst_t;
                            break;
                        }
                    }

                    // insert. find proper position starting from right.
                    for(int j = process_count-1; j >= 0; j--)
                    {
                        if(sorted_process_arr[j].pid == -1) 
                        {
                            if(j == 0) { sorted_process_arr[j] = IO_arr[i];}
                            continue;
                        }
                        
                        if(IO_arr[i].priority < sorted_process_arr[j].priority)
                        {
                            sorted_process_arr[j+1] = IO_arr[i];
                            break;
                        }
                        else
                        {
                            sorted_process_arr[j+1] = sorted_process_arr[j];
                            sorted_process_arr[j] = nothing;
                            if(j==0) { sorted_process_arr[j] = IO_arr[i];}
                        }
                    }

                    IO_arr[i] = nothing;
                }
            }
        }

        // check if IO time or not
        int IO = 0;
        for(int i = 0; i < running.IO_request_count; i++)
        {
            if(running.executed_cpu_t == running.IO_request_t[i]) // it's IO time.
            {
                IO = 1;
                break;
            }
            else if(running.executed_cpu_t < running.IO_request_t[i]) { break; }
        }

        if(eval == 0)
        {
            if (running.pid == -1) { printf("|%6d|%5d|%17d|    |\n", tick, running.pid, running.cpu_burst_t);}
            else if (IO > 0) { printf("|%6d|%5d|%17d| IO |\n", tick, running.pid, running.cpu_burst_t - running.executed_cpu_t); }
            else { printf("|%6d|%5d|%17d|    |\n", tick, running.pid, running.cpu_burst_t - running.executed_cpu_t); }
        }
        
        //progress tick
        tick++;
        
        //check process done
        if(running.executed_cpu_t >= running.cpu_burst_t && running.pid != -1)
        {
            Gantt_note[Gantt_index][0] = running.pid;
            Gantt_note[Gantt_index][1] = tick;
            Gantt_index++;

            running = nothing;
            completed++;
        }
        
        //if IO then switch
        if(IO > 0)
        {
            IO = 0;

            Gantt_note[Gantt_index][0] = running.pid;
            Gantt_note[Gantt_index][1] = tick;
            Gantt_note[Gantt_index][2] = 1;
            Gantt_index++;
            
            for(int i = 0; i < process_count; i++)
            {
                if(IO_arr[i].pid == -1)
                {
                    IO_arr[i] = running;
                    break;
                }
            }
            running = nothing;
        }

        //debug
        /*
        printf("ready queue : ");
        for(int i = 0; i < process_count; i++)
        {
            printf("%d ",sorted_process_arr[i].pid);
        }
        printf("\n");

        printf("IO queue : ");
        for(int i = 0; i < process_count; i++)
        {
            printf("%d ",IO_arr[i].pid);
        }
        printf("\n");
        */
       
    } // scheduling finished

    // draw Gantt chart
    if(eval == 0)
    {
        printf("\n=== Gantt chart ===\n");
        for(int i = 0; i < Gantt_index; i++)
        {
            if(Gantt_note[i][0] == -1) { printf("| EMPTY ~%d |",Gantt_note[i][1]);}
            else
            {
                printf("| P[%d] ~%d",Gantt_note[i][0],Gantt_note[i][1]); 
                if(Gantt_note[i][2] == 0) {printf(" |");}
                else if(Gantt_note[i][2] == 1) {printf("(IO) |");}
            }   
        }
        printf("\n===================\n");
    }
    
    //evaluation
    if(eval == 0) { printf("\n=== Evaluation ===\n"); }
    else { printf("\n[ Nonpreemptive Priority ] "); }
    
    // calculate turnaround with original arr
    int turnaround_sum = 0;
    int temp_turnaround = 0;
    for (int i = 0; i < process_count; i++)
    {
        for (int j = Gantt_index - 1; j >= 0; j--)
        {
            if (Gantt_note[j][0] == process_arr[i].pid)
            {
                temp_turnaround = Gantt_note[j][1];
                break;
            }
        }
        turnaround_sum += (temp_turnaround - process_arr[i].arrival_t);

    }
    printf("Average turnaround : %f | ", (float)turnaround_sum / process_count);

    int waiting_sum = turnaround_sum;
    for (int i = 0; i < process_count; i++)
    {
        waiting_sum -= process_arr[i].cpu_burst_t;
        waiting_sum -= (process_arr[i].IO_burst_t * process_arr[i].IO_request_count);
    }
    printf("Average waiting : %f\n", (float)waiting_sum / process_count);
}

void NewPreemptive_Priority(int eval)
{
    process sorted_process_arr[MAX_PROCESS]; // sorted arr prepare
    for (int i = 0; i < process_count; i++) { sorted_process_arr[i] = process_arr[i]; }

    process IO_arr[process_count]; // IO array prepare
    for (int i = 0; i < process_count; i++) { IO_arr[i] = nothing; }

    int IO_arr_count = 0; // IO array counter

    // sorting
    for (int i = 0; i < process_count - 1; i++)
    {
        for (int j = i + 1; j < process_count; j++)
        {
            // set front to graeter
            if (sorted_process_arr[i].priority < sorted_process_arr[j].priority)
            {
                process temp = sorted_process_arr[i];
                sorted_process_arr[i] = sorted_process_arr[j];
                sorted_process_arr[j] = temp;
            }
        }
    }

    if(eval == 0)
    {
        Process_Printing(sorted_process_arr,process_count);
    }

    // preparing variables
    int completed = 0; // completed processes
    int tick = 0; 
    int Gantt_note[1000][3] = { { 0,0,0},}; // which pid ends when, and IO
    int Gantt_index = 0;
    process running = nothing; // made running process, which saves running process information
    
    if(eval == 0)
    {
        printf("| Tick | PID | remaining burst | IO |\n");
        printf("+======+=====+=================+====+\n");
    }
    
    while(completed < process_count && tick < MAX_TICK)
    {
        // from empty or worse to better
        for(int i = 0 ; i < process_count; i++)
        {
            // empty process cannot be preemptive
            if(sorted_process_arr[i].pid == -1) {break;}

            // check empty or better
            if(sorted_process_arr[i].arrival_t <= tick && (running.pid == -1 || 
                sorted_process_arr[i].priority > running.priority))
            {
                //current running was empty
                if(running.pid == -1) 
                {

                    // Gantt noting for empty
                    if(running.executed_cpu_t > 0)
                    {
                        running.executed_cpu_t = 0;
                        Gantt_note[Gantt_index][0] = running.pid;
                        Gantt_note[Gantt_index][1] = tick;
                        Gantt_index++;
                    }
                    
                    // replacing running
                    running = sorted_process_arr[i];
                    sorted_process_arr[i] = nothing;
                    for(int j = i; j < process_count-1 ; j++)
                    {
                        if(sorted_process_arr[j+1].pid != -1) 
                        {
                            sorted_process_arr[j] = sorted_process_arr[j+1];
                            sorted_process_arr[j+1] = nothing;
                        }
                    }
                }
                else //running normal process
                {
                    // Gantt noting for normal process
                    Gantt_note[Gantt_index][0] = running.pid;
                    Gantt_note[Gantt_index][1] = tick;
                    Gantt_index++;

                    // save current running to put again
                    process temp = running;

                    //replace running, reorganize queue
                    running = sorted_process_arr[i];
                    sorted_process_arr[i] = nothing;
                    for(int j = i; j < process_count-1 ; j++)
                    {
                        //from i, drag the queue
                        if(sorted_process_arr[j+1].pid != -1) 
                        {
                            sorted_process_arr[j] = sorted_process_arr[j+1];
                            sorted_process_arr[j+1] = nothing;
                        }
                    }

                    // insert temp, find proper position starting from right
                    for(int j = process_count-1; j >= 0; j--)
                    {
                        if(sorted_process_arr[j].pid == -1)
                        {
                            if(j == 0) { sorted_process_arr[j] = temp;}
                            continue;
                        }
                        if(temp.priority <  sorted_process_arr[j].priority)
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
                }
            
                // found first is the best always.
                break;
            }
        }

        //process & IO Progress
        running.executed_cpu_t++;

        for (int i = 0; i < process_count; i++)
        {
            if (IO_arr[i].pid != -1)
            {
                IO_arr[i].IO_burst_t--;

                if (IO_arr[i].IO_burst_t == 0) // IO is end. put ready queue again.
                {
                    // reset IO_burst
                    for (int j = 0; j < process_count; j++)
                    {
                        if (IO_arr[i].pid == process_arr[j].pid)
                        {
                            IO_arr[i].IO_burst_t = process_arr[j].IO_burst_t;
                            break;
                        }
                    }

                    // insert. find proper position starting from right.
                    for (int j = process_count - 1; j >= 0; j--)
                    {
                        if (sorted_process_arr[j].pid == -1)
                        {
                            if (j == 0) { sorted_process_arr[j] = IO_arr[i]; }
                            continue;
                        }

                        if (IO_arr[i].priority < sorted_process_arr[j].priority)
                        {
                            sorted_process_arr[j + 1] = IO_arr[i];
                            break;
                        }
                        else
                        {
                            sorted_process_arr[j + 1] = sorted_process_arr[j];
                            sorted_process_arr[j] = nothing;
                            if (j == 0) { sorted_process_arr[j] = IO_arr[i]; }
                        }
                    }

                    IO_arr[i] = nothing;
                }
            }
        }

        // check if IO time or not
        int IO = 0;
        for (int i = 0; i < running.IO_request_count; i++)
        {
            if (running.executed_cpu_t == running.IO_request_t[i]) // it's IO time.
            {
                IO = 1;
                break;
            }
            else if (running.executed_cpu_t < running.IO_request_t[i]) { break; }
        }

        if(eval == 0)
        {
            if (running.pid == -1) { printf("|%6d|%5d|%17d|    |\n", tick, running.pid, running.cpu_burst_t);}
            else if (IO > 0) { printf("|%6d|%5d|%17d| IO |\n", tick, running.pid, running.cpu_burst_t - running.executed_cpu_t); }
            else { printf("|%6d|%5d|%17d|    |\n", tick, running.pid, running.cpu_burst_t - running.executed_cpu_t); }
        }

        //progress tick
        tick++;
        
        //process done
        if(running.executed_cpu_t >= running.cpu_burst_t && running.pid != -1)
        {
            Gantt_note[Gantt_index][0] = running.pid;
            Gantt_note[Gantt_index][1] = tick;
            Gantt_index++;

            running = nothing;
            completed++;
        }

        //if IO then switch
        if (IO > 0)
        {
            IO = 0;

            Gantt_note[Gantt_index][0] = running.pid;
            Gantt_note[Gantt_index][1] = tick;
            Gantt_note[Gantt_index][2] = 1;
            Gantt_index++;

            for (int i = 0; i < process_count; i++)
            {
                if (IO_arr[i].pid == -1)
                {
                    IO_arr[i] = running;
                    break;
                }
            }
            running = nothing;
        }
        //debug
        /*
        printf("ready queue : ");
        for(int i = 0; i < process_count; i++)
        {
            printf("%d ",sorted_process_arr[i].pid);
        }
        printf("\n");
        printf("IO queue : ");
        for(int i = 0; i < process_count; i++)
        {
            printf("%d ",IO_arr[i].pid);
        }
        printf("\n");
        */

    } // scheduling finished

    if(eval == 0)
    {
        // draw Gantt chart
        printf("\n=== Gantt chart ===\n");
        for (int i = 0; i < Gantt_index; i++)
        {
            if (Gantt_note[i][0] == -1) { printf("| EMPTY ~%d |", Gantt_note[i][1]); }
            else
            {
                printf("| P[%d] ~%d", Gantt_note[i][0], Gantt_note[i][1]);
                if (Gantt_note[i][2] == 0) { printf(" |"); }
                else if (Gantt_note[i][2] == 1) { printf("(IO) |"); }
            }
        }
        printf("\n===================\n");
    }      

    //evaluation
    if(eval == 0) { printf("\n=== Evaluation ===\n"); }
    else { printf("\n[ Preemptive priority ] "); }

    // calculate turnaround with original arr
    int turnaround_sum = 0;
    int temp_turnaround = 0;
    for (int i = 0; i < process_count; i++)
    {
        for (int j = Gantt_index - 1; j >= 0; j--)
        {
            if (Gantt_note[j][0] == process_arr[i].pid)
            {
                temp_turnaround = Gantt_note[j][1];
                break;
            }
        }
        turnaround_sum += (temp_turnaround - process_arr[i].arrival_t);

    }
    printf("Average turnaround : %f | ", (float)turnaround_sum / process_count);

    int waiting_sum = turnaround_sum;
    for (int i = 0; i < process_count; i++)
    {
        waiting_sum -= process_arr[i].cpu_burst_t;
        waiting_sum -= (process_arr[i].IO_burst_t * process_arr[i].IO_request_count);
    }
    printf("Average waiting : %f\n", (float)waiting_sum / process_count);
}

void NewRound_Robin(int given_quantum ,int eval)
{
    process sorted_process_arr[MAX_PROCESS]; // sorted arr prepare
    for (int i = 0; i < process_count; i++) { sorted_process_arr[i] = process_arr[i]; }

    process IO_arr[process_count]; // IO array prepare
    for (int i = 0; i < process_count; i++) { IO_arr[i] = nothing; }

    int IO_arr_count = 0; // IO array counter

    // sorting
    for (int i = 0; i < process_count - 1; i++)
    {
        for (int j = i + 1; j < process_count; j++)
        {
            // set front to smaller
            if (sorted_process_arr[i].arrival_t > sorted_process_arr[j].arrival_t)
            {
                process temp = sorted_process_arr[i];
                sorted_process_arr[i] = sorted_process_arr[j];
                sorted_process_arr[j] = temp;
            }
        }
    }

    // printing info
    if(eval == 0)
    {
        Process_Printing(sorted_process_arr,process_count);
    }

    // preparing variables
    int completed = 0; // completed processes
    int tick = 0; 
    int Gantt_note[1000][3] = { { 0,0,0},}; // which pid ends when, and IO
    int Gantt_index = 0;
    int done_quantum = 0;
    process running = nothing; // made running process, which saves running process information
    
    if(eval == 0)
    {
        printf("| Tick | PID | remaining burst | IO |\n");
        printf("+======+=====+=================+====+\n");
    }

    while(completed < process_count && tick < MAX_TICK)
    {
        // from empty to new one
        if (sorted_process_arr[0].arrival_t <= tick && running.pid == -1 && sorted_process_arr[0].pid != -1)
        {
            done_quantum = 0;

            // Gantt noting for empty
            if(running.executed_cpu_t > 0)
            {
                running.executed_cpu_t = 0;
                Gantt_note[Gantt_index][0] = running.pid;
                Gantt_note[Gantt_index][1] = tick;
                Gantt_index++;
            }

            // replace running, reorganize queue
            running = sorted_process_arr[0];
            sorted_process_arr[0] = nothing;
            for (int j = 0; j < process_count - 1; j++)
            {
                // from i, drag the queue
                if (sorted_process_arr[j + 1].pid != -1)
                {
                    sorted_process_arr[j] = sorted_process_arr[j + 1];
                    sorted_process_arr[j + 1] = nothing;
                }
            }  
        }
        else if (done_quantum >= given_quantum)
        {
            done_quantum = 0;

            // Gantt noting for normal process
            Gantt_note[Gantt_index][0] = running.pid;
            Gantt_note[Gantt_index][1] = tick;
            Gantt_index++;
            
            if (sorted_process_arr[0].pid != -1)
            {
                // save current running to put again
                process temp = running;

                if (sorted_process_arr[0].arrival_t <= tick)
                {
                    // replace running, reorganize queue
                    running = sorted_process_arr[0];
                    sorted_process_arr[0] = nothing;
                    for (int j = 0; j < process_count - 1; j++)
                    {
                        // from i, drag the queue
                        if (sorted_process_arr[j + 1].pid != -1)
                        {
                            sorted_process_arr[j] = sorted_process_arr[j + 1];
                            sorted_process_arr[j + 1] = nothing;
                        }
                    }

                    // insert temp. find proper position starting from right.
                    for (int j = process_count - 1; j >= 0; j--)
                    {
                        if (sorted_process_arr[j].pid == -1)
                        {
                            if (j == 0) { sorted_process_arr[j] = temp; }
                            continue;
                        }
                        else { sorted_process_arr[j + 1] = temp; }
                        break;
                    }
                }
            }
        }

        //process & IO Progress & time quantum progress
        running.executed_cpu_t++;
        if (running.pid != -1) { done_quantum++; }
        
        for (int i = 0; i < process_count; i++)
        {
            if (IO_arr[i].pid != -1)
            {
                IO_arr[i].IO_burst_t--;

                if (IO_arr[i].IO_burst_t == 0) // IO is end. put ready queue again.
                {
                    // reset IO_burst
                    for (int j = 0; j < process_count; j++)
                    {
                        if (IO_arr[i].pid == process_arr[j].pid)
                        {
                            IO_arr[i].IO_burst_t = process_arr[j].IO_burst_t;
                            break;
                        }
                    }

                    // insert. find proper position starting from right.
                    for (int j = process_count - 1; j >= 0; j--)
                    {
                        if (sorted_process_arr[j].pid == -1)
                        {
                            if (j == 0) { sorted_process_arr[j] = IO_arr[i]; }
                            continue;
                        }
                        else { sorted_process_arr[j + 1] = IO_arr[i]; }
                        break;
                    }

                    IO_arr[i] = nothing;
                }
            }
        }

        // check if IO time or not
        int IO = 0;
        for (int i = 0; i < running.IO_request_count; i++)
        {
            if (running.executed_cpu_t == running.IO_request_t[i]) // it's IO time.
            {
                IO = 1;
                break;
            }
            else if (running.executed_cpu_t < running.IO_request_t[i]) { break; }
        }

        if(eval == 0)
        {
            if (running.pid == -1) { printf("|%6d|%5d|%17d|    |\n", tick, running.pid, running.cpu_burst_t);}
            else if (IO > 0) { printf("|%6d|%5d|%17d| IO |\n", tick, running.pid, running.cpu_burst_t - running.executed_cpu_t); }
            else { printf("|%6d|%5d|%17d|    |\n", tick, running.pid, running.cpu_burst_t - running.executed_cpu_t); }
        }

        //progress tick
        tick++;
        
        //process done
        if(running.executed_cpu_t >= running.cpu_burst_t && running.pid != -1)
        {
            Gantt_note[Gantt_index][0] = running.pid;
            Gantt_note[Gantt_index][1] = tick;
            Gantt_index++;

            running = nothing;
            completed++;
            done_quantum = 0;
        }

        //if IO then switch
        if (IO > 0)
        {
            IO = 0;

            Gantt_note[Gantt_index][0] = running.pid;
            Gantt_note[Gantt_index][1] = tick;
            Gantt_note[Gantt_index][2] = 1;
            Gantt_index++;

            for (int i = 0; i < process_count; i++)
            {
                if (IO_arr[i].pid == -1)
                {
                    IO_arr[i] = running;
                    break;
                }
            }
            running = nothing;
            done_quantum = 0;
        }
        //debug
        printf("ready queue : ");
        for(int i = 0; i < process_count; i++)
        {
            printf("%d ",sorted_process_arr[i].pid);
        }
        printf("\n");
        printf("IO queue : ");
        for(int i = 0; i < process_count; i++)
        {
            printf("%d ",IO_arr[i].pid);
        }
        printf("\n");
    } // scheduling finished

    // draw Gantt chart
    if(eval == 0)
    {
        printf("\n=== Gantt chart ===\n");
        for (int i = 0; i < Gantt_index; i++)
        {
            if (Gantt_note[i][0] == -1) { printf("| EMPTY ~%d |", Gantt_note[i][1]); }
            else
            {
                printf("| P[%d] ~%d", Gantt_note[i][0], Gantt_note[i][1]);
                if (Gantt_note[i][2] == 0) { printf(" |"); }
                else if (Gantt_note[i][2] == 1) { printf("(IO) |"); }
            }
        }
        printf("\n===================\n");
    }


    //evaluation
    if(eval == 0) { printf("\n=== Evaluation ===\n"); }
    else { printf("\n[ RR ] "); }

    // calculate turnaround with original arr
    int turnaround_sum = 0;
    int temp_turnaround = 0;
    for (int i = 0; i < process_count; i++)
    {
        for (int j = Gantt_index - 1; j >= 0; j--)
        {
            if (Gantt_note[j][0] == process_arr[i].pid)
            {
                temp_turnaround = Gantt_note[j][1];
                break;
            }
        }
        turnaround_sum += (temp_turnaround - process_arr[i].arrival_t);

    }
    printf("Average turnaround : %f | ", (float)turnaround_sum / process_count);

    int waiting_sum = turnaround_sum;
    for (int i = 0; i < process_count; i++)
    {
        waiting_sum -= process_arr[i].cpu_burst_t;
        waiting_sum -= (process_arr[i].IO_burst_t * process_arr[i].IO_request_count);
    }
    printf("Average waiting : %f\n", (float)waiting_sum / process_count);
}