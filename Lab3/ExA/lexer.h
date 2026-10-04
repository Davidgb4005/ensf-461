#ifndef TOKENIZER_H
#define TOKENIZER_H



char * tokenize(char * input,const char * delimeters,char ** cursor);

#ifdef TESTING
int tokenMain();
#endif

#endif