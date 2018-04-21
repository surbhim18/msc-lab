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
           printf("Result: %d\n\n",$1);
         }
         ;

expr:	LBKT expr RBKT {$$ = $2; }  
 	| expr PLUS expr { $$ = $1+$3; }
 	| expr MINUS expr { $$ = $1-$3; }
 	| expr MULTIPLY expr { $$ = $1*$3; }
 	| expr DIVIDE expr { $$ = $1/$3; }
 	| NUM
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
