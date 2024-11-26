#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lexer.h>
#include <parser.h>
#include <keywords.h>
#include <symtab.h>
int lookahead = 0;
int lexlevel = 1; // nivel léxico do programa
int error_count = 0; // contador de erros
int local_init_position = 0; // grava a próxima posição fa definição de procedure e function
int haveFunctionReturn = 0; // grava para saber se encontrou um symtab_lookup onde o retorno dele é igual a local_init_position
// Prototipando todas as funções utilizadas

// Função principal
void program(void) { // initial symbol
    match(PROGRAM);
    match(ID);
    match('(');
    idList(1); // cria como parametro mas como não tem tipo não vai fazer o override
    match(')');
    match(';');
    block(); // implementar a lógica do block (VAR, PROCEDURE, FUNCTION, BEGIN END, etc.)
    match('.');
    
}

void block(void) {
    vardef();
    sbprgdef();
    beginend();
}

void type(int first_term, int last_term){
    switch (lookahead)
    {
        case INTEGER:
            match(INTEGER);
            symtab_type_override(first_term, last_term, 0);
        case REAL:
            match(REAL);
            symtab_type_override(first_term, last_term, 1);
        case LONG:
            match(LONG);
            symtab_type_override(first_term, last_term, 2);
        case DOUBLE:
            match(DOUBLE);
            symtab_type_override(first_term, last_term, 3);
        default:
            match(BOOLEAN);
            symtab_type_override(first_term, last_term, 4);
    }
}

void vardef(void) {
    int tamanhoListIds = 0;
    int typeParm = 0;
    if (lookahead == VAR) {
        match(VAR);
_idlist:
        int first_term_position = symtab_next_entry;
        idList(0); // passar 0 para dentro para criar todos os Ids como variable
        match(':');
        type(first_term_position, symtab_next_entry - 1);
        match(';');
        if (lookahead == ID) {
            goto _idlist;
        }
    }
}

void sbprgdef(void) {
    int error_stat = 0;
    int tipoProg = 0;
    int nameProcFunc[1];
    while (lookahead == PROCEDURE || lookahead == FUNCTION) {
        int isfunc = (lookahead == FUNCTION);
        match(lookahead);
        error_stat = symtab_append(lexeme, lexlevel, isfunc, 0, 0);
        if(error_stat < 0){
            error_count++;
        }
        else
        {
            nameProcFunc[0] = error_stat;
        }
        match(ID);
        local_init_position = symtab_next_entry;
        lexlevel++;
        parmList();
        lexlevel--;
        if (isfunc) {
            match(':');
            type(local_init_position - 1, local_init_position - 1);
        }
        match(';');
        lexlevel++;
        block();
        match(';');
        if(isfunc && !haveFunctionReturn)
        {
            fprintf(stderr, "A Função {%s} não possui retorno.", symtab[local_init_position - 1]);
            error_count++;
        }
        lexlevel--;
        symtab_next_entry = local_init_position;
        local_init_position = 0;
    }
}
void parmList(void) {
    int tamanhoListIds = 0;
    int typeParm = 0;
    if (lookahead == '(') {
        match('(');
_parmlist:
        if(lookahead == PROCEDURE || lookahead == FUNCTION){
            match(lookahead);
            heading();
        }
        else
        {
            if (lookahead == VAR) {
                match(VAR);
            }
            int first_term_position = symtab_next_entry - 1;
            idList(1); // passa 1 para dentro para criar os Ids como parametros
            match(':');
            type(first_term_position, symtab_next_entry - 1);
        }
        if (lookahead == ';') {
            match(';');
            goto _parmlist;
        }
        match(')');
    }
}

void heading(void){
    match(ID);
    parmList();
    if(lookahead == FUNCTION) {
        match(':');
        type(0 , 0);
    }
}

void idList(int typeId) {
    int error_stat = 0;
_idlist:
    error_stat = symtab_append(lexeme, lexlevel, 2, 0, typeId);
    if(error_stat < 0){
        error_count++;
    }
    match(ID);
    if (lookahead == ',') {
        match(',');
        goto _idlist;
    }
}

void beginend(void) {
    match(BEGIN);
    stmlist();
    match(END);

}

void stmlist(void) {
_stmlist:
    stmt();
    if (lookahead == ';') {
        match(';');
        goto _stmlist;
    }
}

void stmt(void) {
    switch (lookahead) {
        case ID:
            idstmt();
            break;
        case BEGIN:
            beginend();
            break;
        case IF:
            ifstmt();
            break;
        case WHILE:
            whlstmt();
            break;
        case REPEAT:
            repstmt();
            break;
        default:
            break;
    }
}

void idstmt(void) {
    int id_position = symtab_lookup(lexeme, lexlevel, 0);
    if (id_position < 0){
        error_count++;
    }
    match(ID);
    if (id_position == local_init_position - 1){
        if(lookahead == ASGN){
            haveFunctionReturn = 1; // atribui que a função sendo escriturada possui retorno
        }
    }
    if (lookahead == ASGN) {
        match(ASGN);
        expr();
    } else {
        exprList();
    }
}

void ifstmt(void) {
    match(IF);
    expr();
    match(THEN);
    stmt();
    if (lookahead == ELSE) {
        match(ELSE);
        stmt();
    }
}

void whlstmt(void) {
    match(WHILE);
    expr();
    match(DO);
    stmt();
}

void repstmt(void) {
    match(REPEAT);
    stmlist();
    match(UNTIL);
    expr();
}

void exprList(void) {
    if (lookahead == '(') {
        match('(');
_exprlist:
        expr();
        if (lookahead == ',') {
            match(',');
            goto _exprlist;
        }
        match(')');
    }
}

void expr(void) {
    smpexpr();
    /*ele vai dar match no retorno do  isRELOP()*/
    if(relop()){
        match(lookahead);
        smpexpr();
    }
}

void smpexpr(void) {
    if (lookahead == '+' || lookahead == '-') {
        match(lookahead);
    }
    term();
    while (lookahead == '+' || lookahead == '-' || lookahead == OR) {
        match(lookahead);
        term();
    }
}

void term(void) {
    factor();  // Você pode implementar essa função conforme necessário
    while (lookahead == '*' || lookahead == '/' || lookahead == DIV || lookahead == MOD || lookahead == AND) {
        match(lookahead);
        factor(); // Implementar também se necessário
    }
}

void factor(void) {
    switch (lookahead) {
        case ID:
            match(ID);
            if (lookahead == ASGN){
                match(ASGN);
                expr();
            } else if (lookahead == '('){
                exprList();
            }
            break;
        case '(':
            match('(');
            expr();
            match(')');
            break;
        case NOT:
            match(NOT);
            factor();
            break;
        case NUM:
            match(NUM);
            break;
    }
}

int relop(void)
{
    switch (lookahead)
    {
        case '>':
        case GEQ:
        case '<':
        case LEQ:
        case NEQ:
        case '=':
            return lookahead;
    }
    return 0;
}

void match(int expected) {
    if (lookahead == expected) {
        lookahead = gettoken(source);
    } else {
        fprintf(stderr, "sintax error at Line: %d, expected %d, received %d\n", linenum, expected, lookahead);
        exit(-3);
    }
}
