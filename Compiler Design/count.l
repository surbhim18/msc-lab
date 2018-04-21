%{
#include<stdio.h>
#include<string.h>

char fname[25];
int chars=0, words=0, newlines=0, tabs=0, whitespaces=0;


%}

%%

"//".*"\n"	newlines++;
[^ \t\n]+	words++; chars+=yyleng;
" "		chars++; whitespaces++;
"\t"		chars++; tabs++;	
\n	        newlines++; chars++;



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

	printf("No. of characters:%d\nNo. of words:%d\nNo. of newlines:%d\nNo. of tabs:%d\nNo. of whitespaces: %d\n",chars,words,newlines,tabs,whitespaces);

	return 0;
}

int yywrap()
{
	return 1;
}
