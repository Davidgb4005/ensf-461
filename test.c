#include "stdio.h"

#define RATIO(x,y) x < y ? x/y : y/x
#define RATIO2(x,y) ((x) < (y) ? (x)/(y) : (y)/(x)) 
int main(){

    int a = 1;
    int rat1 = RATIO(a+1,2);
    int rat2 = RATIO2(a+1,2);
    printf("RATIO = %d RATIO2 = %d",rat1,rat2);
}