#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <limits.h>
#include <time.h>
#define DEBUG 0
#define min(a, b) (((a) < (b)) ? (a) : (b))

// total jobs
int numofjobs = 0;

typedef struct job
{
    // job id is ordered by the arrival; jobs arrived first have smaller job id, always increment by 1
    int id;
    int arrival; // arrival time; safely assume the time unit has the minimal increment of 1
    int length;
    int tickets; // number of tickets for lottery scheduling
    int start_time;
    int finish_time;
    int original_length;
    struct job const *analysis_next;
    struct job *next;
} job;

// the workload list
long cpu_time = 0;
int id_count;
job *head = NULL;
job *head_analysis = NULL;
void update_avg(double *running_avg, job *current_job, int count)
{
    int response = current_job->start_time - current_job->arrival;
    int turnaround = current_job->finish_time - current_job->arrival;
    int wait = turnaround - current_job->original_length;

    printf("Job %d -- Response time: %d  Turnaround: %d  Wait: %d\n",
           current_job->id - 1, response , turnaround +1 , wait+1);

    running_avg[0] += (response - running_avg[0]) / count;
    running_avg[1] += (turnaround - running_avg[1]) / count;
    running_avg[2] += (wait - running_avg[2]) / count;
}
void print_analysis(double *avg,const char *policy)
{
    printf("Average -- Response: %.2f  Turnaround %.2f  Wait %.2f\n",
           avg[0], avg[1]+1, avg[2]+1);
    printf("End analyzing %s.\n", policy);
}

void append_to(job **head, int arrival, int length, int tickets)
{
    id_count++;
    job *cursor = NULL;
    job *node = (job *)calloc(1, sizeof(job));
    node->arrival = arrival;
    node->length = length;
    node->original_length = length;
    node->tickets = tickets;
    node->id = id_count;
    node->start_time = -1;
    node->finish_time = -1;
    if (*head == NULL)
    {
        *head = node;
        return;
    }
    cursor = *head;
    while (cursor->next != NULL)
    {
        cursor = cursor->next;
    }
    cursor->next = node;
    cursor->analysis_next = node;
    return;
}

void read_job_config(const char *filename)
{

    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int tickets = 0;
    char *delim = ",";
    char *arrival = NULL;
    char *length = NULL;

    // TODO, error checking
    fp = fopen(filename, "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    // TODO: if the file is empty, we should just exit with error
    while ((read = getline(&line, &len, fp)) != -1)
    {
        if (line[read - 1] == '\n')
            line[read - 1] = 0;
        arrival = strtok(line, delim);
        length = strtok(NULL, delim);
        tickets += 100;

        append_to(&head, atoi(arrival), atoi(length), tickets);
    }

    fclose(fp);
    if (line)
        free(line);
}

int cpu_tick(job *current_job)
{
    if (current_job != NULL && current_job->arrival <= cpu_time)
    {
        if (current_job->start_time == -1)
        {
            current_job->start_time = cpu_time;
        }
        current_job->length--;
        if (current_job->length == 0)
        {
            current_job->finish_time = cpu_time;
        }
        cpu_time++;
        return 1;
#if DEBUG
        printf("Cpu Time at %ld With Job ID %d : Time Remaining %d\n", cpu_time, current_job->id, current_job->length);
#endif
    }
    else
    {
#if DEBUG
        printf("Cpu Time at %ld with No Current Job <CPU IDLE>\n", cpu_time);
#endif
    }
    cpu_time++;
    return 0;
}

void print_trace(int start_time, job *current_job, int duration)
{
    printf("t=%d: [Job %d] arrived at [%d], ran for: [%d]\n",
           start_time, current_job->id - 1, current_job->arrival, duration);
}

void policy_SJF(job *head)
{
    printf("Execution trace with SJF:\n");
    while (head != NULL)
    {
        L1:
        job *shortest_job = head;
        job *cursor = head;
        job *node_to_shortest = NULL;
        job *prev_node = NULL;
        while (cursor != NULL)
        {
            if (shortest_job->length > cursor->length && cursor->arrival <= cpu_time && cursor->length)
            {
                shortest_job = cursor;
                node_to_shortest = prev_node;
            }
            prev_node = cursor;
            cursor = cursor->next;
        }
        if (shortest_job->arrival > cpu_time){
            cpu_tick(NULL);
            goto L1;
        }
        int duration = 0;
        while (shortest_job->length > 0)
        {
            duration += cpu_tick(shortest_job);
        }
        if (shortest_job == NULL){
            break;
        }
        else if (shortest_job == head)
        {
            head = head->next;
        }
        else if (shortest_job->next != NULL)
        {
            node_to_shortest->next = shortest_job->next;
        }
        else
        {
            node_to_shortest->next = NULL;
        }
        if (duration){
            print_trace(shortest_job->start_time, shortest_job, duration);
        }
    }

    printf("End of execution with SJF.\n");
}

void policy_STCF(job *head)
{
    printf("Execution trace with STCF:\n");
    while (head != NULL)
    {
        job *shortest_job = head;
        job *cursor = head;
        job *node_to_shortest = NULL;
        job *prev_node = NULL;
        while (cursor != NULL)
        {
            if (shortest_job->length > cursor->length && cursor->arrival <= cpu_time && cursor->length)
            {
                shortest_job = cursor;
                node_to_shortest = prev_node;
            }
            prev_node = cursor;
            cursor = cursor->next;
        }
        int new_job_arrival = 0;
        int duration = 0;
        while (shortest_job->length > 0 && !new_job_arrival)
        {
            duration += cpu_tick(shortest_job);
            cursor = head;
            while (cursor != NULL)
            {
                if (cursor->arrival == cpu_time)
                {
                    new_job_arrival = 1;
                    break;
                }
                cursor = cursor->next;
            }
        }
        if (duration)
        {
            print_trace(shortest_job->start_time, shortest_job, duration);
        }
        if (shortest_job->length == 0)
        {
            if (shortest_job == head)
            {
                head = head->next;
            }
            else if (shortest_job->next != NULL)
            {
                node_to_shortest->next = shortest_job->next;
            }
            else
            {
                node_to_shortest->next = NULL;
            }
        }
    }

    printf("End of execution with STCF.\n");
}

void policy_RR(int slice, job *head)
{
    printf("Execution trace with RR:\n");
    job *cursor = head;
    int work_done = 1;
    while (work_done)
    {
        int time_slice = slice;
        work_done = 0;
        int duration = 0;
        while (cursor->length > 0 && time_slice > 0 && cursor->arrival <= cpu_time)
        {
            cpu_tick(cursor);
            time_slice--;
            work_done++;
            duration++;
        }
        if (duration)
        {
            print_trace(cursor->start_time, cursor, duration);
        }
        if (cursor->next == NULL)
        {
            cursor = head;
        }
        else
        {
            cursor = cursor->next;
        }
    }

    printf("End of execution with RR.\n");
}

void policy_LT(int slice)
{
    printf("Execution trace with LT:\n");

    // Leave this here, it will ensure the scheduling behavior remains deterministic
    srand(42);

    // In the following, you'll need to:
    // Figure out which active job to run first
    // Pick the job with the shortest remaining time
    // Considers jobs in order of arrival, so implicitly breaks ties by choosing the job with the lowest ID

    // To achieve consistency with the tests, you are encouraged to choose the winning ticket as follows:
    // int winning_ticket = rand() % total_tickets;
    // And pick the winning job using the linked list approach discussed in class, or equivalent

    printf("End of execution with LT.\n");
}

void policy_FIFO(job *head)
{
    printf("Execution trace with FIFO:\n");

    while (head != NULL)
    {
        int duration = 0;
        while (head->length > 0)
        {
            duration += cpu_tick(head);
        }
        print_trace(head->start_time, head, duration);
        head = head->next;
    }
    printf("End of execution with FIFO.\n");
}

int main(int argc, char **argv)
{

    static char usage[] = "usage: %s analysis policy slice trace\n";

    int analysis;
    char *pname;
    char *tname;
    int slice;

    if (argc < 5)
    {
        fprintf(stderr, "missing variables\n");
        fprintf(stderr, usage, argv[0]);
        exit(1);
    }

    // if 0, we don't analysis the performance
    analysis = atoi(argv[1]);

    // policy name
    pname = argv[2];

    // time slice, only valid for RR
    slice = atoi(argv[3]);

    // workload trace
    tname = argv[4];
    read_job_config(tname);

    const char *policy_names[] = {"FIFO", "SJF", "STCF", "RR", "LT"};
    int selected_policy = -1;

    if (strcmp(pname, "FIFO") == 0)
    {
        selected_policy = 0;
        policy_FIFO(head);
    }
    else if (strcmp(pname, "SJF") == 0)
    {
        selected_policy = 1;
        policy_SJF(head);
    }
    else if (strcmp(pname, "STCF") == 0)
    {
        selected_policy = 2;
        policy_STCF(head);
    }
    else if (strcmp(pname, "RR") == 0)
    {
        selected_policy = 3;
        policy_RR(slice, head);
    }
    else if (strcmp(pname, "LT") == 0)
    {
        selected_policy = 4;
        // TODO
    }

    if (analysis == 1 && selected_policy != -1)
    {
        double avg_array[3] = {0};
        int count = 0;
        printf("Begin analyzing %s:\n", policy_names[selected_policy]);

        while (head != NULL)
        {
            update_avg(avg_array, head, ++count);
            head = head->analysis_next;
        }

        print_analysis(avg_array, policy_names[selected_policy]);
    }

    exit(0);
}
