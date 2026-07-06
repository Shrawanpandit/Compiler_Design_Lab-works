%{
#include "y.tab.h"
#include <stdlib.h>
#include <stdio.h>
%}

%%

[0-9]+ {
    yylval.num = atoi(yytext);
    return NUMBER;
}

[ \t]+ ;

\n {
    return '\n';
}

[()+\-*/] {
    return yytext[0];
}

. {
    printf("Unknown character: %s\n", yytext);
}

%%

int yywrap(void)
{
    return 1;
}