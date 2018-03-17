/*
   Authors:	Surbhi Mittal
*/

#include<dirent.h> 
#include<stdio.h> 
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>
#include<string.h>


void visit(char *dir_name)
{    
    DIR *d;				
    struct dirent *dir;
    d = opendir(dir_name);		//open the directory in program

    chdir(dir_name);			//change pointer to current directory
    printf("%s\n",dir_name);
    system("ls -l");			//print contents of directory
    printf("\n");
   
    if (d)				//if directory exists 
        {
            while ((dir = readdir(d)) != NULL)	// till there is something to read in the directory
            {
                  if(dir->d_type != DT_REG && strcmp(dir->d_name,".")!=0 && strcmp(dir->d_name,"..")!=0) //if it contains other directories
                  {   	
                      visit(dir->d_name);		//visit the directory
                  }
            }
        }

    if(strcmp(dir_name,".")!=0)
	chdir("..");			// point to correct current directory again

    closedir(d);
}


int main(void)
{
    printf("Current directory\n");
    visit(".");
        
    return(0);
}
