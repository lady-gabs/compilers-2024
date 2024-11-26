#include <ctype.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdio.h>
#include <lexer.h>

#define MAXIDLEN 32

int linenum;
char lexeme[MAXIDLEN + 1];

int isASGN(FILE *tape)
{
    lexeme[0] = getc(tape);
    if (lexeme[0] == ':') { // verifica se ele começa o ':'
        lexeme[1] = getc(tape);
        if (lexeme[1] == '=') { // verifica se tem '=' depois do ':'
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
        while (isalnum(lexeme[i] = getc(tape))) i++; // verifica se é id
        ungetc(lexeme[i], tape);
        lexeme[i] = '\0';
        return ID;
    }

    ungetc(lexeme[0], tape);
    return 0;
}

// DEC = [1-9][0-9]* | 0
// 1234567890
// 0012 = 0 0 12
int isDEC(FILE *tape)
{
    lexeme[0] = getc(tape);

    if (isdigit(lexeme[0])) { //se for digito
        int i = 1;
        while (isdigit(lexeme[i] = getc(tape))) i++; // enquanto for digito
        ungetc(lexeme[i], tape);
        lexeme[i] = '\0'; 
        return DEC;
    }

    ungetc(lexeme[0], tape);
    return 0;
}

//OCT = 0[0-7]+
// Ex: 00 | 01 | 02 ....
int isOCT(FILE *tape)
{
    lexeme[0] = getc(tape);

    if (lexeme[0] == '0') {
        int i = 1;
        if (isdigit(lexeme[i] = getc(tape))) {
            if (lexeme[i] < '0' || lexeme[i] > '7') { // oct precisa ser entre 0 e 7
                ungetc(lexeme[i], tape);
                return 0;
            }
            while (isdigit(lexeme[i] = getc(tape))) {
                if (lexeme[i] < '0' || lexeme[i] > '7') { // também precisa ser 0 e 7
                    ungetc(lexeme[i], tape);
                    return 0; // caso falhe em qualquer digito,e ele retorna uma falha
                }
                i++;
            }
            ungetc(lexeme[i], tape);
            lexeme[i] = '\0';
            return OCT;
        }
        ungetc(lexeme[i], tape);
    }

    ungetc(lexeme[0], tape);
    return 0;
}

// HEX = 0[xX][0-9a-fA-F]+
// 0X1 0X2 0x3 0x4 0x11
int isHEX(FILE *tape)
{
    lexeme[0] = getc(tape);

    if (lexeme[0] == '0') {
        int i = 1;
        if (lexeme[i] = getc(tape)) {
            if (lexeme[i] == 'x' || lexeme[i] == 'X') { // hexadecimal precisa que ele tenha 'x'
                i++;
                if (isxdigit(lexeme[i] = getc(tape))) { //valida se é um digito hexadecimal
                    while (isxdigit(lexeme[i] = getc(tape))) i++;
                    ungetc(lexeme[i], tape);
                    lexeme[i] = '\0';
                    return HEX;
                }
            }
            ungetc(lexeme[i], tape);
        }
    }

    ungetc(lexeme[0], tape);
    return 0;
}

double lexval;
int isNUM(FILE *tape){
    int hasDecimal = 0;
    lexeme[0] = getc(tape);

    if (isdigit(lexeme[0])) { // Verifica se é digito
        int i = 1;
        
        while (isdigit(lexeme[i] = getc(tape))) i++; // enquanto for digito

        if (lexeme[i] == '.') { // Verifica se é ponto flutuante
            hasDecimal = 1;
            i++;
            while (isdigit(lexeme[i] = getc(tape))) i++; // enquanto for digito
        }

        if (lexeme[i] == 'e' || lexeme[i] == 'E') { // se tiver exponencial
            i++;
            lexeme[i] = getc(tape);
            if (lexeme[i] == '+' || lexeme[i] == '-') { // verifica se ele é para divir ou multiplicar o exponencial
                i++;
            }
            if (!isdigit(lexeme[i])) { // caso tenha erro no exponencial ele vai dar token mismath
                // ungetc(lexeme[i], tape);
                // ungetc(lexeme[i - 1], tape); // Retorna o sinal, se necessário
                // if (hasDecimal) {
                //     lexeme[i] = '\0';
                //     return FLOAT;
                // } else {
                //     return 0;
                // }
                ungetc(lexeme[i], tape);
                return 0;
            }
            i++;
            while (isdigit(lexeme[i] = getc(tape))) i++;
        }

        ungetc(lexeme[i], tape);
        lexeme[i] = '\0';
        return NUM;
    }

    ungetc(lexeme[0], tape);
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
    // if ((token = isOCT(source))) return token;
    // if ((token = isHEX(source))) return token;
    // if ((token = isDEC(source))) return token;
    if ((token = isNUM(source))) return token;
    
    token = getc(source);
    return token;
}
