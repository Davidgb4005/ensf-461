#ifndef __PARSER_H
#define __PARSER_H
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <stdbool.h>
#include "trie.h"


typedef struct execution
{
    char *cmd;
    char **args;
    int operation;
    struct execution *next_exec;
} execution;


//ToDo: Here are some suggested parsing commands, you may want to expand this list, 
//Please feel free to change modify the commands below to suit your needs

execution *parse_command(char *input,trie_node * root);

#endif
