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
#include <time.h>
#include <stdio.h>
#include <sys/sysmacros.h>

void printFileType(struct stat sb)
{
    switch (sb.st_mode & S_IFMT)
    {
    case S_IFBLK:
        printf("Block Device\n");
        break;
    case S_IFCHR:
        printf("Character Device\n");
        break;
    case S_IFDIR:
        printf("Directory\n");
        flag=1;
        break;
    case S_IFIFO:
        printf("FIFO/pipe\n");
        break;
    case S_IFLNK:
        printf("Symbolic Link\n");
        break;
    case S_IFREG:
        printf("Regular File\n");
        break;
    case S_IFSOCK:
        printf("Socket\n");
        break;
    default:
        printf("unknown?\n");
        break;
    }

}

void printPermissions(struct stat sb)
{
    printf( (S_ISDIR(sb.st_mode)) ? "d" : "-");
    printf( (sb.st_mode & S_IRUSR) ? "r" : "-");
    printf( (sb.st_mode & S_IWUSR) ? "w" : "-");
    printf( (sb.st_mode & S_IXUSR) ? "x" : "-");
    printf( (sb.st_mode & S_IRGRP) ? "r" : "-");
    printf( (sb.st_mode & S_IWGRP) ? "w" : "-");
    printf( (sb.st_mode & S_IXGRP) ? "x" : "-");
    printf( (sb.st_mode & S_IROTH) ? "r" : "-");
    printf( (sb.st_mode & S_IWOTH) ? "w" : "-");
    printf( (sb.st_mode & S_IXOTH) ? "x" : "-");
}

void visit(char *file)
{
    struct stat sb;
    int flag=0;
    char path[1000];

    if (lstat(file, &sb) == -1)
    {
        perror("lstat");
        exit(EXIT_FAILURE);
    }

    getcwd(path,sizeof(path));      // prints absolute path to the file
    printf("File name:		  %s/%s\n",path,file);
    printf("File type:                ");
    printFileType(sb);
    printf("File size:                %lld bytes\n", (long long) sb.st_size);
    printf("User ID:		  %d\n", sb.st_uid);
    printf("Group ID:		  %d\n", sb.st_gid);
    printf("File Permissions: \t");
    printPermissions(sb);
    printf("\n");
    printf("Last file access:         %s", ctime(&sb.st_atime));
    printf("Last file modification:   %s", ctime(&sb.st_mtime));
    printf("\n");

    if(flag==1)             // if the file is a directory
    {
        DIR *d;
        struct dirent *dir;
        d = opendir(file);		//open the directory in program
        chdir(file);

        if (d)				//if directory exists
        {
            while ((dir = readdir(d)) != NULL)	// till there is something to read in the directory
            {
                if(strcmp(dir->d_name,".")!=0 && strcmp(dir->d_name,"..")!=0) //if the file is not self or parent
                {
                    visit(dir->d_name);		//visit the directory
                }
            }
        }

        if(strcmp(file,".")!=0)
            chdir("..");			// point to correct current directory again

        closedir(d);
    }
}

int main(void)
{
    printf("Current directory\n");
    visit(".");     // Start from this directory

    return(0);
}
