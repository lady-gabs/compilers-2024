#include <stdio.h>
#include <stdlib.h>
#include <symtab.h>
#include <string.h>
#include <lexer.h>

SYMTAB symtab[MAXSTBSIZE];
int symtab_next_entry = 0;

int symtab_append(char const* word, int level, int objType, int type, int parmFlag){
    if(symtab_lookup(word, level, 1) < 0){
        //printf("NAME: %s |LEVEL: %d |Tipo Objeto: %d |Type: %d|ParamOrVar: %d\n", word, level, objType, type, parmFlag);
        strcpy(symtab[symtab_next_entry].name, word);
        symtab[symtab_next_entry].lexlevel = level;
        symtab[symtab_next_entry].objtype = objType;
        symtab[symtab_next_entry].type = type;
        symtab[symtab_next_entry].parmflag = parmFlag;
        symtab_next_entry++;
        return symtab_next_entry - 1;
    }
    fprintf(stderr, "Fatal error: symbol {%s} already defined in the same or higher level than{%d}\n", word, level);
    return -1;
}
// source_function_caller == 0 => using parameter in code
// source_function_caller != 0 => search parameter to include by symtab_append
int symtab_lookup(char const* word, int level, int source_function_caller){
    int i = 0;
    for (i = symtab_next_entry - 1; i > -1; i--){
        if(strcmp(symtab[i].name, word) == 0 && symtab[i].lexlevel <= level){
            return i;
        }
    }
    if(source_function_caller == 0)
        fprintf(stderr, "Fatal error: symbol {%s} not defined in level{%d}\n", word, level);
    return -1;
}

void  symtab_type_override(int first_term, int last_term, int Tipo){
    for (int i = first_term; i < last_term; i++)
    {
        symtab[i].type = Tipo;
        //printf("\tOVERRIDE => NAME: %s |LEVEL: %d |Tipo Objeto: %d |Type: %d|ParamOrVar: %d\n", symtab[symtab_entry[i]].name, symtab[symtab_entry[i]].lexlevel, symtab[symtab_entry[i]].objtype, symtab[symtab_entry[i]].type, symtab[symtab_entry[i]].parmflag);
    }
}