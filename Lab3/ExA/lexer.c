#include "stdlib.h"
#include "stdio.h"
#include "string.h"

int charIsDelimeter(char c,const char * delimeters){
    while(*delimeters){
        if(c == *delimeters){
            return 1;
        }
        delimeters++;
    }
    return 0;
}
void charCleanUpAndClear(char * tkn,char clean_up_char){
    int tkn_count = 0;
    //char * debug_string = tkn;
    //int len = strlen(tkn);
    while(*(tkn+tkn_count)){
        if(*(tkn+tkn_count) == clean_up_char){
            tkn_count++;
            if (!(*(tkn+tkn_count))){
                break;
            }
        }
        *tkn = *(tkn+tkn_count);
        tkn++;
    }
    while(*tkn){
        *tkn = ' ';
        tkn++;
    }
}
void charCleanUp(char *tkn,char clean_up_char)
{
    char *read_ptr = tkn;
    char *write_ptr = tkn;
    while (*read_ptr)
    {
        if (*read_ptr != clean_up_char)
        {
            *write_ptr = *read_ptr;
            write_ptr++;
        }
        read_ptr++;
    }
    *write_ptr = '\0';
}
char * tokenize(char * input,const char * delimeters,char ** cursor){
    int dir_quote = 0;
    if(input != NULL){
        *cursor = input;
    }
    while(charIsDelimeter(**cursor,delimeters)){
        (*cursor)++;
    }
    if(!(**cursor)){
        return NULL;
    }
    char * tkn_start = *cursor;
    while(**cursor){
        if(**cursor == '"'){
            dir_quote ^= 1;
        }
        if(charIsDelimeter(**cursor,delimeters) && !dir_quote){
            **cursor = '\0';
            charCleanUp(tkn_start,'"');
            (*cursor)++;
            return tkn_start;
        }
        (*cursor)++;
    }
    **cursor = '\0';
    charCleanUp(tkn_start,'"');
    return tkn_start;
}

#ifdef TESTING
int tokenMain(){
    char my_str[] = "This Is a\"Test String F\"or    the P\"arse r\"";
    const char * delimeter = " \0";
    char * token;
    char * cursor;
    token = tokenize(my_str,delimeter,&cursor);
    while(token != NULL){
        printf("%s\n",token);
        token = tokenize(NULL," ",&cursor);
    }
}
#endif
