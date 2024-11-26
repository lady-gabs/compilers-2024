#include <stdio.h>
#include <stdlib.h>

extern FILE *source;
extern int lookahead;
extern int error_count;

extern void match(int expected);
void program(void);
void block(void);
void vardef(void);
void sbprgdef(void);
void parmList(void);
void idList(int);
void beginend(void);
void stmlist(void);
void stmt(void);
void idstmt(void);
void ifstmt(void);
void whlstmt(void);
void repstmt(void);
void type(int, int);
void exprList(void);
void expr(void);
void smpexpr(void);
void term(void);
void factor(void);
void heading(void);
int relop(void);
