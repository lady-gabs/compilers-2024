/*
#include <ctype.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdio.h>
#include <lexer.h>

int linenum;
char lexeme[MAXIDLEN + 1];
double lexval;

int isASGN(FILE *tape)
{
    lexeme[0] = getc(tape);
    if (lexeme[0] == ':') {
        lexeme[1] = getc(tape);
        if (lexeme[1] == '=') {
            lexeme[2] = '\0';
            return ASGN;
        } else {
            ungetc(lexeme[1], tape);
        }
    }
    ungetc(lexeme[0], tape);
    return 0;
}


// ID = [A-Za-z][A-Za-z0-9]*
// Ex: abacaXI123
int isID(FILE *tape)
{
    lexeme[0] = getc(tape);

    if (isalpha(lexeme[0])) {
        int i = 1;
        while (isalnum(lexeme[i] = getc(tape))) i++;
        ungetc(lexeme[i], tape);
        lexeme[i] = '\0';
        return ID;
    }

    ungetc(lexeme[0], tape);
    return 0;
}

int isNUM(FILE *tape){
    int nc;
    nc = fscanf(tape, "%f", lexval);
    if(nc) return NUM;
    return 0;
}

void skipspaces(FILE *tape)
{
    int c;
    while (isspace(c = getc(tape))) {
        if (c == '\n') linenum++;
    }
    ungetc(c, tape);
}

int gettoken(FILE *source)
{
    int token;
    skipspaces(source);
    if ((token = isASGN(source))) return token;
    if ((token = isID(source))) return token;
    if ((token = isNUM(source))) return token;
    
    token = getc(source);
    return token;
}

*/