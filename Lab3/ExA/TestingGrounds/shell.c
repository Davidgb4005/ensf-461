#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "exec_wrappers.h"
#define BUFLEN 1024
#define TESTING 1
extern int process_count;

int main(void)
{
    trie_node *root = calloc(1, sizeof(trie_node));

    trieAddOperator(root, "|", op_pipe);
    trieAddOperator(root, "|&", pipe_stdout_stderr);

    char input[BUFLEN];

    while (1)
    {
        process_count = 0;
        printf("$ ");
        #if TESTING == 0
            if (fgets(input, sizeof(input), stdin) == NULL)
            {
                break;
            }
        #else
            static int i = 0;

            const char *tests[] = {
                "echo hello",
                "echo hello world",
                "echo \"hello world\"",
                "echo hell\"o world\"",
                "echo hello | grep hello",
                "echo hello | grep -o ell",
                "echo \"hello world\" | grep -o hello | cat",
                "echo \"hello world\" | grep hello | grep -o world",
                "ls /this/path/does/not/exist | grep XYZ",
                "ls /this/path/does/not/exist |& grep XYZ",
                "ls /this/path/does/not/exist |& grep cannot",
                "ls /this/path/does/not/exist |& cat",
                "this_command_does_not_exist",
                "exit"
            };

            const int test_count = sizeof(tests) / sizeof(tests[0]);

            if (i >= test_count) {
                break;
            }

            printf("TEST: %s\n", tests[i]);
            strcpy(input, tests[i]);
            i++;

        #endif
        input[strcspn(input, "\n")] = '\0';

        if (strcmp(input, "exit") == 0)
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