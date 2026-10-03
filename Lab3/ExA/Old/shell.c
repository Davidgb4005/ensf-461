#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "parser.h"

#define BUFLEN 1024

// To Do: This base file has been provided to help you start the lab, you'll need to heavily modify it to implement all of the features

int main(void)
{
    char input[] = "ls -l | tee output.txt | grep rwx";
    execution *cmd = parse_command(input);
    int process_counter = 0;
    int prev_read = -1;

    while (cmd != NULL)
    {
    {
        int pipe_fd[2];
        // If Another Ps Node exists create a pipe
        if (cmd->next_cmd != NULL)
        {
            pipe(pipe_fd);
        }
        pid_t pid = fork();
        process_counter++;
        if (pid == 0)
        {

            // if its not the first process pipe std in
            if (prev_read != -1)
            {
                dup2(prev_read, STDIN_FILENO);
                close(prev_read);
            }
            // if its not the last process pipe std out
            if (cmd->next_cmd != NULL)
            {
                close(pipe_fd[0]);
                dup2(pipe_fd[1], STDOUT_FILENO);
                close(pipe_fd[1]);
            }
            execvp(cmd->cmd, cmd->args);
        }
        else
        {
            // close read for first process pipe never needed
            if (prev_read != -1)
            {
                close(prev_read);
            }
            // close the write for the last process never needed
            if (cmd->next_cmd != NULL)
            {
                close(pipe_fd[1]);
                prev_read = pipe_fd[0];
            }
        }
        cmd = cmd->next_cmd;
    }
    }
    for (int i; i < process_counter; i++)
    {
        wait(NULL);
    }
    printf("%d", process_counter);
    return 1;
}
