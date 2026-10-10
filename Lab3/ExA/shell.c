#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "exec_wrappers.h"

#define BUFLEN 1024

extern int process_count;
int test_enabled = 0;
int main(void)
{
    trie_node *root = calloc(1, sizeof(trie_node));

    trieAddOperator(root, "|", op_pipe);
    trieAddOperator(root, "|&", pipe_stdout_stderr);

    char input[BUFLEN];

    printf("Welcome to the Group 24 shell! Enter commands, enter 'quit' to exit\n");
    
    while (1)
    {
        process_count = 0;
        printf("$ ");
        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            break;
        }

        input[strcspn(input, "\n")] = '\0';
        if (strcmp(input, "quit") == 0 || strcmp(input, "exit") == 0)
        {     
            trieFree(root);
            break;
        }

        int return_pipe = 0;
        int prev_op = no_op;

        execution *cmd = parse_command(input, root);

        while (cmd != NULL)
        {
            switch (cmd->operation)
            {
            case op_pipe:
                process_count++;
                prev_op = op_pipe;

                if (process_count == 1)
                {
                    return_pipe = pipeOutExecv(cmd);
                }
                else if (cmd->next_exec != NULL)
                {
                    return_pipe = pipeInOutExecv(cmd, return_pipe);
                }
                break;
            case pipe_stdout_stderr:
                process_count++;
                prev_op = pipe_stdout_stderr;

                if (process_count == 1)
                {
                    return_pipe = pipeOutErrExecv(cmd);
                }
                else if (cmd->next_exec != NULL)
                {
                    return_pipe = pipeInOutErrExecv(cmd, return_pipe);
                }

                break;
            case no_op:
            default:
                switch (prev_op)
                {
                case op_pipe:
                    pipeInExecv(cmd, return_pipe);
                    break;

                case pipe_stdout_stderr:
                    pipeInErrExecv(cmd, return_pipe);
                    break;

                default:
                    defaultExecv(cmd);
                    break;
                }

                break;
            }
            free(cmd->args);
            execution *doomed_node = cmd;
            cmd = cmd->next_exec;
            free(doomed_node);
        }
        for (int i = 0; i <process_count; i++)
        {
            wait(NULL);
        }

    }


    return 0;
}