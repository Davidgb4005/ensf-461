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
    trie_node * root = calloc(1,sizeof(trie_node));
    trieAddOperator(root,"|",op_pipe);//Add as valid shell operator to trie
    int process_counter = 0;

    char input[1024];

while (1) {
    printf("$ ");

    if (fgets(input, sizeof(input), stdin) == NULL) {
        break;
    }

    input[strcspn(input, "\n")] = '\0';

    if (strcmp(input, "bye") == 0) {
        break;
    }

    execution *cmd = parse_command(input,root);
    int prev_read = -1;
    process_counter = 0;
    while (cmd != NULL)
    {
        int pipe_fd[2];
        // If Another Ps Node exists create a pipe
        if (cmd->next_exec != NULL)
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
            if (cmd->next_exec != NULL)
            {
                close(pipe_fd[0]);
                dup2(pipe_fd[1], STDOUT_FILENO);
                close(pipe_fd[1]);
            }
            execvp(cmd->cmd, cmd->args);
            perror("execvp");
            _exit(127);
        }
        else if(pid > 0)
        {
            // close read for first process pipe never needed
            if (prev_read != -1)
            {
                close(prev_read);
            }
            // close the write for the last process never needed
            if (cmd->next_exec != NULL)
            {
                close(pipe_fd[1]);
                prev_read = pipe_fd[0];
            }
        }
        else{
            perror("fork");
            printf("Invald Command");
        }
        free(cmd->args);
        execution * doomed_node = cmd;
        cmd = cmd->next_exec;
        free(doomed_node);
    }
    for (int i = 0; i < process_counter; i++)
    {
        wait(NULL);
    }



}while(strcmp(input,"bye"));

    trieFree(root); 
    printf("%d", process_counter);
    return 1;
}

