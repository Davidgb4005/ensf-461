#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "exec_wrappers.h"

const char *resolve_path(const char *cmd)
{
    static char full_path[1024];
    if (strchr(cmd, '/') != NULL)
    {
        if (access(cmd, X_OK) == 0)
            return cmd;

        return NULL;
    }
    const char *path = getenv("PATH");
    if (path == NULL)
        return NULL;
    char path_copy[4096];
    if (strlen(path) >= sizeof(path_copy))
        return NULL;
    strcpy(path_copy, path);
    char *dir = strtok(path_copy, ":");
    while (dir != NULL)
    {
        int ret = snprintf(
            full_path,
            sizeof(full_path),
            "%s/%s",
            dir,
            cmd);

        if (ret < 0 || (long unsigned int)ret >= sizeof(full_path))
        {
            dir = strtok(NULL, ":");
            continue;
        }

        if (access(full_path, X_OK) == 0)
        {
            return full_path;
        }

        dir = strtok(NULL, ":");
    }

    return NULL;
}

int process_count = 0;
int pipeOutExecv(execution *cmd)

{
    int pipe_fd[2];
    if (pipe(pipe_fd) == -1)
    {
        perror("pipe");
        return -1;
    }
    pid_t pid = fork();
    if (pid == 0)
    {
        close(pipe_fd[0]);
        dup2(pipe_fd[1], STDOUT_FILENO);
        close(pipe_fd[1]);

        const char *path = resolve_path(cmd->cmd);
        if (path == NULL)
        {
            fprintf(stderr, "%s: command not found\n", cmd->cmd);
            _exit(127);
        }
        execv(path, cmd->args);
        perror("execv");
        _exit(127);
    }

    else if (pid > 0)
    {
        process_count++;
        close(pipe_fd[1]);
    }
    else
    {
        perror("fork");
        printf("Invald Command");
    }

    return pipe_fd[0];
}
void pipeInExecv(execution *cmd, int in_pipe)
{

    pid_t pid = fork();
    if (pid == 0)
    {

        dup2(in_pipe, STDIN_FILENO);
        close(in_pipe);
        const char *path = resolve_path(cmd->cmd);
        
        if (path == NULL)
        {
            fprintf(stderr, "%s: command not found\n", cmd->cmd);
            _exit(127);
        }

        execv(path, cmd->args);
        perror("execv");
        _exit(127);
    }

    else if (pid > 0)
    {
        process_count++;
        close(in_pipe);
    }
    else
    {
        perror("fork");
        printf("Invald Command");
    }
}

int pipeInOutExecv(execution *cmd, int in_pipe)
{
    int pipe_fd[2];
    if (pipe(pipe_fd) == -1)
    {
        perror("pipe");
        return -1;
    }
    pid_t pid = fork();
    if (pid == 0)
    {
        dup2(in_pipe, STDIN_FILENO);
        close(in_pipe);
        dup2(pipe_fd[1], STDOUT_FILENO);
        close(pipe_fd[1]);

        const char *path = resolve_path(cmd->cmd);
        if (path == NULL)
        {
            fprintf(stderr, "%s: command not found\n", cmd->cmd);
            _exit(127);
        }
        execv(path, cmd->args);
        perror("execv");
        _exit(127);
    }

    else if (pid > 0)
    {
        process_count++;
        close(pipe_fd[1]);
        close(in_pipe);
    }
    else
    {
        perror("fork");
        printf("Invald Command");
    }

    return pipe_fd[0];
}
int pipeOutErrExecv(execution *cmd)
{
    int pipe_fd[2];

    if (pipe(pipe_fd) == -1)
    {
        perror("pipe");
        return -1;
    }

    pid_t pid = fork();

    if (pid == 0)
    {
        close(pipe_fd[0]);

        dup2(pipe_fd[1], STDOUT_FILENO);
        dup2(pipe_fd[1], STDERR_FILENO);

        close(pipe_fd[1]);

        const char *path = resolve_path(cmd->cmd);
        if (path == NULL)
        {
            fprintf(stderr, "%s: command not found\n", cmd->cmd);
            _exit(127);
        }
        execv(path, cmd->args);
        perror("execv");
        _exit(127);
    }
    else if (pid > 0)
    {
        process_count++;
        close(pipe_fd[1]);
    }
    else
    {
        perror("fork");
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        return -1;
    }

    return pipe_fd[0];
}

void pipeInErrExecv(execution *cmd, int in_pipe)
{
    pid_t pid = fork();

    if (pid == 0)
    {
        dup2(in_pipe, STDIN_FILENO);
        close(in_pipe);

        const char *path = resolve_path(cmd->cmd);
        if (path == NULL)
        {
            fprintf(stderr, "%s: command not found\n", cmd->cmd);
            _exit(127);
        }
        execv(path, cmd->args);
        perror("execv");
        _exit(127);
    }
    else if (pid > 0)
    {
        process_count++;
        close(in_pipe);
    }
    else
    {
        perror("fork");
    }
}

int pipeInOutErrExecv(execution *cmd, int in_pipe)
{
    int pipe_fd[2];

    if (pipe(pipe_fd) == -1)
    {
        perror("pipe");
        return -1;
    }

    pid_t pid = fork();

    if (pid == 0)
    {
        close(pipe_fd[0]);
        dup2(in_pipe, STDIN_FILENO);
        close(in_pipe);
        dup2(pipe_fd[1], STDOUT_FILENO);
        dup2(pipe_fd[1], STDERR_FILENO);

        close(pipe_fd[1]);

        const char *path = resolve_path(cmd->cmd);
        if (path == NULL)
        {
            fprintf(stderr, "%s: command not found\n", cmd->cmd);
            _exit(127);
        }
        execv(path, cmd->args);
        perror("execv");
        _exit(127);
    }
    else if (pid > 0)
    {
        process_count++;
        close(in_pipe);
        close(pipe_fd[1]);
    }
    else
    {
        perror("fork");
        close(in_pipe);
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        return -1;
    }

    return pipe_fd[0];
}
void defaultExecv(execution *cmd)
{
    pid_t pid = fork();
    if (pid == 0)
    {
        const char *path = resolve_path(cmd->cmd);
        if (path == NULL)
        {
            fprintf(stderr, "%s: command not found\n", cmd->cmd);
            _exit(127);
        }
        execv(path, cmd->args);
        perror("execv");
        _exit(127);
    }
    else if (pid > 0)
    {
        process_count++;
    }
    else
    {
        perror("fork");
    }
}

