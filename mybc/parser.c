#include <lexer.h>
#include <parser.h>
#include <string.h> //para o symtab_file

#define SYMTABSIZE 256
#define STACKSIZE 1024

int lookahead;
double acc;

char symtab[SYMTABSIZE][MAXIDLEN+1];
double vmem[SYMTABSIZE];
double stack[STACKSIZE];
int symtab_next_entry = 0;
int sp = 0;

void stack_push(double value){ // para fazer o push na stack
    stack[sp++] = value; // Incrementa após o push
}

double stack_pop(){ // para fazer o pop dela
    return stack[--sp]; // Decrementa antes de acessar o valor
}

int symtab_lookup(char *varname){ // busca no symtab qual o local do vmem[] que está o varname passado
    for (int i = 0; i < symtab_next_entry; i++) {
        if (strcmp(varname, symtab[i]) == 0) {
            return i;
        }
    }
    // Se não encontrou, insere nova variável e incrementa o symtab_next_entry
    strcpy(symtab[symtab_next_entry], varname);
    return symtab_next_entry++;
}

void symtab_store(char *varname, double value){ // guarda o valor de varname no local exato do vmem[]
    int i = symtab_lookup(varname);
    vmem[i] = value;
}

double symtab_recall(char *varname){ // retorna o valor de varname de vmem[]
    int i = symtab_lookup(varname);
    if (i != symtab_next_entry) {
        return vmem[i];
    }
    return 0;
}

// E -> ['+' | '-'] T R
void E(void) {
    int oplus = 0;
    int ominus = 0;

    if (lookahead == '+' || lookahead == '-') {
        if (lookahead == '-') ominus = 1; // Se for negativo, sinalizamos para posterior fazer a operação de negacao
        match(lookahead);
    }

_T:
    T();
    
    if (ominus) {
        printf("\tNEG acc\n");
        acc = -acc; // nega o valor de acc
        ominus = 0;
    }

    if (lookahead == '+' || lookahead == '-') {
        oplus = lookahead;
        printf("\tPUSH acc\n");
        stack_push(acc); // faz o push na stack do valor de acc para ser usado na próxima conta
        match(lookahead); 
        goto _T;          
    }

    if (oplus) {
        switch (oplus) {
            case '+':
                printf("\tADD stack[sp--], acc\n"); // adicionar e deixa o resultado no stack
                stack[sp-1] += acc;
                break;
            case '-':
                printf("\tSUB stack[sp--], acc\n"); // subtrair e deixa o resultado no stack
                stack[sp-1] -= acc; 
                break;
        }
        printf("\tPOP acc\n");
        acc = stack_pop(); // retorna o valor do resultado da conta para o acc
        oplus = 0;
    }
}

// T -> F Q
void T(void) {
    int otimes = 0;

_F:
    F();
    
    if (lookahead == '*' || lookahead == '/') {
        otimes = lookahead;
        printf("\tPUSH acc\n");
        stack_push(acc); // faz o push na stack do valor de acc para ser usado na próxima conta
        match(lookahead);
        goto _F;
    }

    if (otimes) {
        switch (otimes) {
            case '*':
                printf("\tMUL stack[sp--], acc\n");
                stack[sp-1] *= acc; // multipla e deixa o resultado no stack
                break;
            case '/':
                printf("\tDIV stack[sp--], acc\n");
                stack[sp-1] /= acc; // divide e deixa o resultado no stack
                break;
        }
        printf("\tPOP acc\n");
        acc = stack_pop(); // retorna o valor do resultado da conta para o acc
        otimes = 0;
    }
}

// F -> (E) | DEC | OCT | ID
void F(void) {
    char varname[MAXIDLEN+1];
    switch (lookahead) {
        case '(':
            match('(');
            E();
            match(')');
            break;
        case NUM:
            acc = atof(lexeme); // interpreta o valor de lexeme retornado
            printf("\tMOV %f, acc\n", acc);
            match(NUM);
            break;
        // case DEC:
        //     acc = atof(lexeme);
        //     printf("\tMOV %f, acc\n", acc);
        //     match(DEC);
        //     break;
        default:
            strcpy(varname, lexeme);
            match(ID);
            if (lookahead == ASGN) {
                // L-value (atribuição de variável)
                match(ASGN);
                E(); // para o caso de ter uma expressão depois ele guarda tudo dentro dela para dentro do E
                printf("\tSTORE acc, %s\n", varname); // store do valor de ID no vmem[]
                symtab_store(varname, acc);
            } else {
                // R-value (recuperação de variável)
                printf("\tRECALL acc, %s\n", varname); // retorno o valor de ID salvo no vmem[]
                acc = symtab_recall(varname);
            }
    }
}

void match(int expected) {
    if (lookahead == expected) {
        lookahead = gettoken(source);
    } else {
        fprintf(stderr, "token mismatch\n");
        exit(-3);
    }
}
