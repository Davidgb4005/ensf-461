#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int ioutmain(void)
{
    int pipefd[2];

    pipe(pipefd);

    pid_t pid = fork();

    if (pid == 0) {
        // Child: read from pipe
        close(pipefd[1]);

        char buffer[100];

        int n = read(pipefd[0], buffer, sizeof(buffer) - 1);
        buffer[n] = '\0';

        printf("Child received: %s\n", buffer);

        close(pipefd[0]);
    }
    else {
        // Parent: write to pipe
        close(pipefd[0]);

        write(pipefd[1], "hello", 5);

        close(pipefd[1]);

        wait(NULL);
    }

    return 0;
}