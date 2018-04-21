%{
     #include <stdio.h>
     #include <stdlib.h>
%}

%start list

%token PLUS MINUS DIVIDE MULTIPLY NUM EOL

%left PLUS MINUS
%left MULTIPLY DIVIDE
%left LBKT RBKT

%%


list:     /*empty */ 
         | list stat EOL
         ;

stat:    expr
         {
           printf("\n\n");
         }
         ;

expr:	LBKT expr RBKT 
 	| expr PLUS expr { printf("+"); }
 	| expr MINUS expr { printf("-");}
 	| expr MULTIPLY expr { printf("*"); }
 	| expr DIVIDE expr { printf("/"); }
 	| NUM { printf("%d",$1); }
 	;

%%
int main()
{
     return(yyparse());
}

int yywrap()
{
	return 1;
}

yyerror(s)
char* s;
{
	fprintf(stderr,"%s\n\n",s);
}
