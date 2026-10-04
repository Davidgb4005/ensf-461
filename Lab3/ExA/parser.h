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




// Parses command input into a linked list of execution nodes.
execution *parse_command(char *input,trie_node * root);

#endif
