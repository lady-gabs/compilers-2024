#pragma once
#include <defineConstants.c>

extern int symtab_append(char const*, int, int, int, int);
extern int symtab_lookup(char const*, int, int);
extern void symtab_type_override(int, int, int);

typedef struct _symtab_
{
    char name[MAXIDLEN];
    int objtype; // 0 => procedure; 1 => function; 2 => variable; CHECK
    int type; // 0 => int32; 1 => int64 = 1; 2 => float32; 3 => float 64; 4 => boolean;
    int lexlevel; // nivel léxico do symbol CHECK
    int parmflag; // 0 => variable; 1 => parameter; CHECK
    char offset[MAXIDLEN];
}SYMTAB;

extern SYMTAB symtab[];
extern int symtab_next_entry;

/*
EDI => 32 bits
RSI => 64 bits
 */