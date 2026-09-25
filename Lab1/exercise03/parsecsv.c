#include <stdio.h>
#include <string.h>
#include "record_list.h"
#include "util.h"


int main(int argc, char** argv) {

    char usage[] = "Usage: parsecsv.out <input CSV file> <output CSV file>\n\n";
    char foerr[] = "Error: unable to open/create file\n\n";

    if ( argc != 3 ) {
        fprintf(stderr, "Usage: parsecsv.out <input CSV file> <output CSV file>\n\n");
        return -1;
    }

    FILE* fin = fopen(argv[1], "r");
    if ( fin == NULL ) {
        fprintf(stderr, "Error: unable to open file %s\n\n", argv[1]);
        return -2;
    }

    int* newline = read_next_line(fin);
    record_t* head = NULL;
    record_t* curr = NULL;
    
    while ( newline != NULL ) {
        float avg = compute_average(newline);
        float sdv = compute_stdev(newline);
        curr = append(curr, avg, sdv);
        if ( head == NULL )
            head = curr;
        free(newline);
        newline = read_next_line(fin);
    }
    fclose(fin);

    FILE* file = fopen(argv[2], "w");

    record_t * node = head;
    while(node != 0){
        fprintf(fin,"%f,%f\n",node->avg,node->sdv);
        node = node->next;
    }

    record_t * doomed_node = head;
    while(doomed_node != 0){
        record_t * next_node = doomed_node->next;
        free(doomed_node);
        doomed_node = next_node;
    }
    head = 0;
    fclose(file);

    return 0;
}