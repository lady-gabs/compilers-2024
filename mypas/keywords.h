#include <stdio.h>
#include <stdlib.h>

enum{
    BEGIN = 0X1000,
    PROGRAM,
    VAR,
    PROCEDURE,
    FUNCTION,
    INTEGER,
    REAL,
    LONG,
    DOUBLE,
    BOOLEAN,
    IF,
    THEN,
    ELSE,
    WHILE,
    DO,
    REPEAT,
    UNTIL,
    DIV,
    MOD,
    AND,
    OR,
    NOT,
    VARIABLE,
    FUNCTION_ID,
    GEQ,
    LEQ,
    NEQ,
    IN,
    HEADING,
    END
};

extern char *keyword[];
extern int isKeyword(char *word);