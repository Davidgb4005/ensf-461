#ifndef __EXEC_WRAPPER_H
#define __EXEC_WRAPPER_H

#include "parser.h"

// Executes a command with stdout redirected to a new pipe and returns the read end.
int pipeOutExecv(execution *cmd);

// Executes a command with stdin redirected from an existing pipe.
void pipeInExecv(execution *cmd, int in_pipe);

// Executes a command using an existing pipe for stdin and a new pipe for stdout.
int pipeInOutExecv(execution *cmd, int in_pipe);

// Executes a command with both stdout and stderr redirected to a new pipe.
int pipeOutErrExecv(execution *cmd);

// Executes the final command in a stdout/stderr pipeline using the supplied pipe as stdin.
void pipeInErrExecv(execution *cmd, int in_pipe);

// Executes a command using an existing pipe for stdin and redirects stdout/stderr to a new pipe.
int pipeInOutErrExecv(execution *cmd, int in_pipe);

// Executes a command without pipe redirection.
void defaultExecv(execution *cmd);

#endif
