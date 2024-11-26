enum {
    ID = 1024,
    NUM,
    ASGN,
};

extern int linenum;
extern char lexeme[];
extern double lexval;

#include <stdio.h>
#include <string.h>

extern int gettoken(FILE *);