#include <string.h>
#include <keywords.h>

char *keyword[] = {
    "begin",
    "program",
    "var",
    "procedure",
    "function",
    "integer",
    "real",
    "long",
    "double",
    "boolean",
    "if",
    "then",
    "else",
    "while",
    "do",
    "repeat",
    "until",
    "div",
    "mod",
    "and",
    "or",
    "not",
    "variable",
    "function_id",
    "geq",
    "leq",
    "neq",
    "in",
    "heading",
    "end"
};

int isKeyword(char *word){
    for(int i = BEGIN; i <= END; i++){
        if(strcmp(word, keyword[i - BEGIN]) == 0){
            return i;
        }
    }
    return 0;
}