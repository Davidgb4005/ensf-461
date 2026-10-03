#include "parser.h"
#include "stdint.h"
#include "string.h"




#include <stdlib.h>
#include <string.h>
#define MAX_CMD_SIZE 64
#define MAX_INPUT_SIZE 1024
#define DELIMETER_COUNT 4

static const char * implmented_operations = {"|>"};

const char * getNextOp(char * input){
    while(input){
        for(int i = 0;implmented_operations[i];i++){
            if(*input == implmented_operations[i])
            return implmented_operations+i;
        }
        input++;
    }
    return NULL;
}

execution * parse_command(char *input)
{
    if (strlen(input)>MAX_INPUT_SIZE){
        fprintf(stderr, "Max input size exceeded\n");
        exit(1);
    }
    execution *head = NULL;
    execution *tail = NULL;
    char *pipe_ptr; //walking ptr for pipe
    const char *next_operation = getNextOp(input);
    char *command = strtok_r(input, implmented_operations, &pipe_ptr);

    //Create N tokens seperated by | characters each token will become a
    //node of the Linked list execution structs
    while (command != NULL) {
        if (strlen(command)>MAX_CMD_SIZE){
            fprintf(stderr, "Max command size exceeded\n");
            exit(1);
        }
        //Create M tokens seperated by spaces the first token becomes the command
        //value all subsecuent become the args
        execution *node = malloc(sizeof(execution));
        node->args = malloc(sizeof(char *) * MAX_CMD_SIZE);
        node->next_cmd = NULL;
        node->operation = next_operation;
        int i = 0;
        char *arg_ptr; // walking ptr for args
        char *token = strtok_r(command, " ", &arg_ptr);
        while (token != NULL) {
            node->args[i] = token;
            i++;
            token = strtok_r(NULL, " ", &arg_ptr);
        }
        node->args[i] = NULL;
        node->cmd = node->args[0];
        if (head == NULL) {
            head = node;
        }
        else {
            tail->next_cmd = node;
        }
        tail = node;
    next_operation = getNextOp(command);
    command = strtok_r(NULL, "|", &pipe_ptr);
    }

    return head;
}


