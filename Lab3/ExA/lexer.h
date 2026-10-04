#ifndef TOKENIZER_H
#define TOKENIZER_H

// Splits an input string into tokens while preserving delimiters inside quotes.
char * tokenize(char * input,const char * delimeters,char ** cursor);

#if TESTING == 1
int tokenMain();
#endif

#endif
