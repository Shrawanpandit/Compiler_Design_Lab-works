%{
#include <stdio.h>
#include <stdlib.h>
int yylex(void);
void yyerror(const char *s);
%}
/* Define semantic value type */
%union {
 int num;
}
/* Token declarations */
%token <num> NUMBER
/* Non-terminal value types */
%type <num> expr term factor
%%
/* Starting symbol */
input:
 /* empty */
 | input line
 ;
line:
 '\n'
 | expr '\n'
 {
 printf("Result = %d\n", $1);
 }
 ;
/* Addition and subtraction */
expr:
 expr '+' term
 {
 $$ = $1 + $3;
 }
 | expr '-' term
 {
 $$ = $1 - $3;
 }
 | term
 {
 $$ = $1;
 }
 ;
/* Multiplication and division */
term:
 term '*' factor
 {
 $$ = $1 * $3;
 }
 | term '/' factor
 {
 if ($3 == 0)
 {
 yyerror("Division by zero");
 $$ = 0;
 }
 else
 {
 $$ = $1 / $3;
 }
 }
 | factor
 {
 $$ = $1;
 }
 ;
/* Parentheses and numbers */
factor:
 '(' expr ')'
 {
 $$ = $2;
 }
 | NUMBER
 {
 $$ = $1;
 }
 ;
%%
/* Error handling function */
void yyerror(const char *s)
{
 fprintf(stderr, "Error: %s\n", s);
}
/* Main function */
int main(void)
{
 printf("=================================\n");
 printf(" Arithmetic Expression Parser\n");
 printf("=================================\n");
 printf("Examples:\n");
 printf(" 3+4*2\n");
 printf(" (3+4)*2\n");
 printf(" 10/5+7\n");
 printf("\nPress Ctrl+Z (Windows) or Ctrl+D (Linux) to exit.\n\n");
 yyparse();
 return 0;
}
