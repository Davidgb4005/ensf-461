void policy_LT(int slice)
{
    printf("Execution trace with LT:\n");

   int arrival; // arrival time; safely assume the time unit has the minimal increment of 1
    int length;
    int tickets; // number of tickets for lottery scheduling
    struct job *prev;
    struct job *next;
}job;

// the workload list
long cpu_time = 0;
int id_count;
job * head = NULL;

void append_to(job ** head, int arrival, int length, int tickets){
    id_count++;
    job * cursor = NULL;
    job * node = (job *)calloc(1,sizeof(job));
    job * prev_node = NULL;
    node->arrival = arrival;
    node->length = length;
    node -> tickets = tickets;
    node -> id = id_count;
    if(*head == NULL){
        *head = node;
        return;
    }
    cursor = *head;
    while(cursor->next != NULL){
        prev_node = cursor;
        cursor = cursor->next;
    }
    cursor->next = node;
    cursor->prev = prev_node;
    return;
}


void read_job_config(const char* filename)
{
    

    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int tickets  = 0;
    char* delim = ",";
    char *arrival = NULL;
    char *length = NULL;

    // TODO, error checking
    fp = fopen(filename, "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    // TODO: if the file is empty, we should just exit with error
    while ((read = getline(&line, &len, fp)) != -1)
    {
        if( line[read-1] == '\n' )
            line[read-1] =0;
        arrival = strtok(line, delim);
        length = strtok(NULL, delim);
        tickets += 100;

        append_to(&head, atoi(arrival), atoi(length), tickets);
    }

    fclose(fp);
    if (line) free(line);
}

void cpu_tick(job * current_job){
    cpu_time ++;
    if(current_job != NULL && current_job ->arrival <= cpu_time){
        current_job->length--;
        #if DEBUG
        printf("Cpu Time at %ld With Job ID %d : Time Remaining %d\n",cpu_time,current_job->id,current_job->length);
        #endif
    }
    else{
        #if DEBUG
        printf("Cpu Time at %ld with No Current Job <CPU IDLE>\n",cpu_time);
        #endif
    }
}

void print_trace(long start, job *current_job, int duration){
    printf("t=%ld: [Job %d] arrived at [%d], ran for: [%d]\n",
           cpu_time, current_job->id - 1, current_job->arrival, duration);
}

void policy_SJF(job * head)
{
    printf("Execution trace with SJF:\n");
    while(head != NULL){
        job * shortest_job = head;
        job * cursor = head;
        job * node_to_shortest = NULL;
        job * prev_node = NULL;
        while(cursor != NULL){
            if (shortest_job->length>cursor->length && cursor->arrival<=cpu_time){
                shortest_job = cursor;
                node_to_shortest = prev_node;
            }
            prev_node = cursor;
            cursor=cursor->next;
        }
        long start = cpu_time;
        int duration = shortest_job->length;
        while(shortest_job->length > 0){
            cpu_tick(shortest_job);
        }
        print_trace(start, shortest_job, duration);
        if(shortest_job == head){
            head = head ->next;
        }
        else if (shortest_job-> next != NULL){
            node_to_shortest -> next = shortest_job ->next;
        }
        else{
            node_to_shortest->next = NULL;
        }
    }

    printf("End of execution with SJF.\n");

}


void policy_STCF(job * head)
{
    printf("Execution trace with STCF:\n");
    while(head != NULL){
        job * shortest_job = head;
        job * cursor = head;
        job * node_to_shortest = NULL;
        job * prev_node = NULL;
        while(cursor != NULL){
            if (shortest_job->length>cursor->length && cursor->arrival<=cpu_time){
                shortest_job = cursor;
                node_to_shortest = prev_node;
            }
            prev_node = cursor;
            cursor=cursor->next;
        }
        int new_job_arrival = 0;
        long start = cpu_time;
        while(shortest_job->length > 0 && !new_job_arrival){
            cpu_tick(shortest_job);
            cursor = head;
            while(cursor != NULL){
                if (cursor->arrival == cpu_time){
                    new_job_arrival = 1;
                    break;
                }
                cursor = cursor->next;

            }
        }
        if(cpu_time > start){
            print_trace(start, shortest_job, (int)(cpu_time - start));
        }
        if(shortest_job == head){
            head = head ->next;
        }
        else if (shortest_job-> next != NULL){
            node_to_shortest -> next = shortest_job ->next;
        }
        else{
            node_to_shortest->next = NULL;
        }
    }


    printf("End of execution with STCF.\n");
}


void policy_RR(int slice,job * head)
{
    printf("Execution trace with RR:\n");
    int time_slice = slice;
    job * prev = NULL;
    job * cursor = head;
    int work_done = 1;
    while(work_done){
        work_done = 0;
        long start = cpu_time;
        while(cursor-> length > 0 && time_slice > 0 && cursor->arrival <= cpu_time){
            cpu_tick(head);
            time_slice--;
            work_done++;
        }
        if(cpu_time > start){
            print_trace(start, cursor, (int)(cpu_time - start));
        }
        if(cursor->next == NULL){
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


void policy_FIFO(job * head){
    printf("Execution trace with FIFO:\n");

    while(head != NULL){
        long start = cpu_time;
        int duration = head->length;
        while(head->length > 0){
            cpu_tick(head);
        }
        print_trace(start, head, duration);
        head = head->next;
    }
    printf("End of execution with FIFO.\n");
}


int main(int argc, char **argv){

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

    if (strcmp(pname, "FIFO") == 0){
        policy_FIFO(head);
        if (analysis == 1){
            // TODO: perform analysis
        }
    }
    else if (strcmp(pname, "SJF") == 0)
    {
        policy_SJF(head);
    }
    else if (strcmp(pname, "STCF") == 0)
    {
        policy_STCF(head);
    }
    else if (strcmp(pname, "RR") == 0)
    {
        policy_RR(slice,head);
    }
    else if (strcmp(pname, "LT") == 0)
    {
        // TODO
    }

	exit(0);
}