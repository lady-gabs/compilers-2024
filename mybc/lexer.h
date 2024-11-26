enum{
    ID = 1024,
    DEC,
    OCT,
    HEX,
    NUM,
    ASGN, // ASGN = ":="
    FLT,
};

#define MAXIDLEN 32
extern int linenum;
extern char lexeme[];
extern double lexval;
// #define ID 1024
// #define DEC 1025
// #define OCT 1026
// #define HEX 1027


#include <stdio.h>
#include <string.h>

extern int gettoken(FILE *);
