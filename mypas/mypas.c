#include <stdio.h>
#include <parser.h>
#include <lexer.h>

/**
 * Bruno Granado Manfrinato da Silva Oliveira
 * Gabriel Passos de Oliveira
 * Gabriella Alves de Oliveira
 */

FILE *source;

int main(int argc, char *argv[])
{
    char nome_arq[100];

    // Verifica se o nome do arquivo foi passado como argumento
    if (argc != 2) {
        printf("Uso: %s <nome_do_arquivo>\n", argv[0]);
        return 1; // Retorna erro se o nome do arquivo não foi fornecido
    }

    // Colocar o nome passado por parêmetro para o nome_arq
    snprintf(nome_arq, sizeof(nome_arq), "%s", argv[1]);
    
    //Tenta abrir o arquivo, se der erro ele devolve erro
    source = fopen(nome_arq, "rt");
    if (source == NULL) {
        perror("Erro ao abrir o arquivo");
        return 1; // Retorna erro se o arquivo não puder ser aberto
    }
    lookahead = gettoken(source);
    program();
    
    if(error_count > 0){
        printf("\n\tERROR IN COMPILATION, CHECK ERROR MESSAGES ABOVE!\n");
    }
    else{
        printf("\n\t=====COMPILADO COM SUCESSO!!!!=====\n");
    }
    return 0;
}