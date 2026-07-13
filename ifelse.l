%{
#include <stdio.h>
#include <stdlib.h>
#include "ifelse.tab.h"
%}

%%

"if"                        { return IF; }
"else"                      { return ELSE; }
"="                         { return ASSIGN; }
";"                         { return SEMICOLON; }

"("                         { return '('; }
")"                         { return ')'; }

[<>!=]=?                    { return RELOP; }

[0-9]+                      {
                                yylval = atoi(yytext);
                                return NUM;
                            }

[a-zA-Z_][a-zA-Z0-9_]*      {
                                return ID;
                            }

[ \t\r]+                    {
                                /* Ignore spaces and tabs */
                            }

\n                          {
                                return '\n';
                            }

.                           {
                                printf("Unknown symbol: %s\n", yytext);
                            }

%%

int yywrap(void)
{
    return 1;
}