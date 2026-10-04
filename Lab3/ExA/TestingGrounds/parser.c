#include "parser.h"
#include "stdint.h"
#include "string.h"
#include "lexer.h"


#include <stdlib.h>
#include <string.h>
#define MAX_CMD_SIZE 128



execution * parse_command(char *input,trie_node * root)
{

    execution *tail = NULL;
    execution *node = malloc(sizeof(execution));
    execution *head = node;
    node->args = malloc(sizeof(char *) * MAX_CMD_SIZE);
    node->next_exec = NULL;
    char * cursor;
    char * token = tokenize(input, " ",&cursor);
    int i = 0;
    while (token != NULL) {
        operator_e op_code = trieSearch(root,token);
        if (op_code != no_op){
            tail = node;
            tail->cmd = node->args[0];
            tail->args[i] = NULL;
            tail->operation = op_code;
            node = malloc(sizeof(execution));
            tail->next_exec = node;
            tail = node;
            node->args = malloc(sizeof(char *) * MAX_CMD_SIZE);
            node->next_exec = NULL;
            node->operation = no_op;
            i = 0;
            token = tokenize(NULL, " ",&cursor);
        }
        node->args[i] = token;
        i++;
        token = tokenize(NULL, " ",&cursor);
    }

    tail = node;
    tail->cmd = node->args[0];
    tail->args[i] = NULL;
    tail->operation = no_op;

    return head;
}


