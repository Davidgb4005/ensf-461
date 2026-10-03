#include "stdlib.h"
#include "stdio.h"
#include "string.h"
void quoteCleanUp(char * tkn){
    int tkn_count = 0;
    int shift = 0;
    char * debug_string = tkn;
    int len = strlen(tkn);
    while(*(tkn+tkn_count)){
        if(*(tkn+tkn_count) == '"'){
            tkn_count++;
            if (!(*(tkn+tkn_count))){
                while(*tkn){
                    *tkn = '\0';
                    tkn++;
                }
                return;
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
char * tokenize(char * input,const char * delimeters,char ** cursor){
    const char * delimeter_cursor = delimeters;
    int dir_quote = 0;
    int dir_quote_cnt = 0;
    if(input != NULL){
        *cursor = input;
    }
    int ignore_leading_delims = 1;
    while(1){
        if(!(**cursor)){
            return NULL;
        }
        while(*delimeter_cursor){
            ignore_leading_delims = 0;
            if(**cursor == *delimeter_cursor){
                ignore_leading_delims = 1;
            }
            delimeter_cursor++;
        }
        delimeter_cursor = delimeters;
        if(!ignore_leading_delims){
            break;
        }
        (*cursor)++;
    }
    char * tkn_start = *cursor;

    while(**cursor){
        if(**cursor == '"'){
            dir_quote ^= 1;
            dir_quote_cnt++;
        }
        while(*delimeter_cursor && !dir_quote){
            if(**cursor == *delimeter_cursor){
                **cursor = '\0';
                if(dir_quote_cnt){
                    quoteCleanUp(tkn_start);
                }
                (*cursor)++;
                return tkn_start;
            }
            delimeter_cursor++;
        }
        delimeter_cursor = delimeters;
        (*cursor)++;
    }
    **cursor = '\0';
    if(dir_quote_cnt){
        quoteCleanUp(tkn_start);
    }
    return tkn_start;
}


int AMmain(){
    char my_str[] = "This Is a\"Test String F\"or the P\"arse r\"";
    const char * delimeter = " \0";
    char * token;
    char * cursor;
    token = tokenize(my_str,delimeter,&cursor);
    while(token != NULL){
        printf("%s\n",token);
        token = tokenize(NULL," ",&cursor);
    }
}