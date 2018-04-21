%{

#include <stdio.h>
#include "y.tab.h"

int c;
extern int yylval;

%}

%%

"exit"			{ exit(0);	}
"\n"			{ return (EOL); }
"("			{ return (LBKT);}
")"			{ return (RBKT);}
"+"			{ return (PLUS); }
"-"			{ return (MINUS);}
"*"			{ return (MULTIPLY);}
"/"			{ return (DIVIDE); }
[0-9]+  		{ yylval =atoi(yytext); return(NUM);  }
.			{ ; }

%%

