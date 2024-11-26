#include <ctype.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdio.h>
#include <lexer.h>
#include <keywords.h>
#include <defineConstants.c>


int linenum = 1;
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

    int i = 1;
    if (isalpha(lexeme[0])) {
        while (isalnum(lexeme[i] = getc(tape))) i++; // verifica se é id
        ungetc(lexeme[i], tape);
        lexeme[i] = '\0';
        if(i = isKeyword(lexeme)){ // Verifica se o que foi encontrado é uma palavra chave
            return i; // retorna o valor que a palavra chave tem no keywords.h para o lookahead
        }
        return ID; // retorna com o valor padrão de ID para o lookahead
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
            lexeme[i] = getc(tape);
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

int isRELOP(FILE *tape){
    int head = getc(tape);
    lexeme[0] = head;
    
    switch (head) {
    case '=':
        lexeme[1] = getc(tape);
        if (lexeme[1] == '=') {
            lexeme[2] = '\0';
            return '=';  // "==""
        }
        ungetc(lexeme[1], tape);
        break;
        
    case '>':
        lexeme[1] = getc(tape);
        if (lexeme[1] == '=') {  // checar se é '>='
            lexeme[2] = '\0';
            return GEQ;  // ">="
        }
        ungetc(lexeme[1], tape);
        lexeme[1] = '\0';
        return '>';  // ">"
        
    case '<':
        lexeme[1] = getc(tape);
        if (lexeme[1] == '=') {  // checar se é '<='
            lexeme[2] = '\0';
            return LEQ;  // "<="
        }
        ungetc(lexeme[1], tape);
        lexeme[1] = '\0';
        return '<';  // "<"
        
    case '!':  // Verifica se o operador começa com '!'
        lexeme[1] = getc(tape);
        if (lexeme[1] == '=') {
            lexeme[2] = '\0';
            return NEQ;  // "!="
        }
        ungetc(lexeme[1], tape);
        break;
    }
    
    ungetc(head, tape);
    return 0;
}

void skipspaces(FILE *tape)
{
    int head;
_skipspaces:
    while (isspace(head = getc(tape))) {
        if (head == '\n') linenum++;
    }
    if (head == '{') {
        // Encontramos um comentário de bloco, então ignoramos até encontrar o fechamento '}'
        while ((head = getc(tape)) != '}');
        goto _skipspaces;  // Volta para processar o próximo caractere
    }
    ungetc(head, tape);
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
