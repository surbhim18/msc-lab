%{
#include<stdio.h>
#include<string.h>

char fname[25];

%}

%%

[a-zA-Z]+ {
		for(int i=0; i<yyleng; i++)
			printf("%c", yytext[yyleng-i-1]);
		
	  }

%%

int main(int argc, char *argv[])
{
	if(argc!=2)
	{
		printf("Inappropriate arguments!\n");
		return 1;
	}
	
	strcpy(fname,argv[1]);
	yyin = fopen(fname,"r");	

	yylex();
	return 0;
}

int yywrap()
{
	return 1;
}
