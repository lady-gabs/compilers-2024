#include <stdio.h>
#include <parser.h>
#include <lexer.h>

/**
 * Bruno Granado Manfrinato da Silva Oliveira
 * Gabriel Passos de Oliveira
 * Gabriella Alves de Oliveira
 */

FILE *source;

int main()
{
    source = stdin;
    lookahead = gettoken(source);
    E();
    printf("\n\tRESULTADO: %lf\n",acc);
    return 0;
}
