#ifndef __EXEC_WRAPPER_H
#define __EXEC_WRAPPER_H

#include "parser.h"


int pipeOutExecv(execution *cmd);
void pipeInExecv(execution *cmd, int in_pipe);
int pipeInOutExecv(execution *cmd, int in_pipe);
int pipeOutErrExecv(execution *cmd);
void pipeInErrExecv(execution *cmd, int in_pipe);
int pipeInOutErrExecv(execution *cmd, int in_pipe);
void defaultExecv(execution *cmd);


#endif