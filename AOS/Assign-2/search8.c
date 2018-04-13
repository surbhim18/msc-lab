/*
 * Implementation of Distributed Linear Search using Linux System calls
 * An array is split up into sub-arrays and searching the sub-arrays for an integer is delegated to other processes.
 */

#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include<sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include<string.h>

/* Global declaration of array and array size. */
int *arr, size;


/* Filling array using file data.
 * First line of the array file assumed to contain the number of elements in the array. */

void getarr(char *fname)
{
    FILE *fp;
    char str[200];
    int x;
    
    fp = fopen(fname, "r");
    
    if (fp == NULL)
    {
        printf("Could not open file %s!\n",fname);
        exit(1);
    }

    fgets(str, sizeof(str), fp);
    size = atoi(str);
    
    int i;
    arr = (int*)malloc(size*sizeof(int));
    
    for(i=0; i<size; i++)
    {
        fgets(str, sizeof(str), fp);
        arr[i] = atoi(str);
    }
    
    /* Printing array */
    printf("Size: %d\n",size);
    
    for(i=0; i<size; i++)
    {
        printf("%d ",arr[i]);
    }

    fclose(fp);
    
}

/* Program for linear search; returns index of element, if found */
int linear_search(int *arr, int start, int end, int val)
{
    int i;
    
    if(start < 0 || end >= size || start > end)
        return -1;
    
    for(i=start; i<=end; i++)
    {
        if(arr[i] == val)
            return i;
    }
    
    return -1;
}

int to_chil_1[2];  /* information to child process 1 */
int to_chil_2[2];  /* information to child process 2 */
int to_par[2];

//int distributed_search(int *arr, int start, int end, int val)
void distributed_search(int *arr, int start, int end, int val)
{
    
    int index;
    int arrsize = end - start +1;
    
     if(start < 0 || end >= size || start > end)
        exit(0);
    
     if(arrsize > 5)
     {
         
        pid_t pid1, pid2;
        
        int start1 = start; int end1 = start + (arrsize/2)-1;
        int start2 = start + (arrsize/2); int end2 = end;
        
        pid_t pid;
        pid = fork();
    
        if(pid < 0)
        {
            printf("Error creating process!\n");
            exit(1);
        }
        
        
        if(pid == 0)         /* child 1 created */
        {
            pid1 = getpid();
            printf("Inside child 1\n");
        
            //close(to_chil_1[1]);                       /* close write end to child */
        
            int start, end;
        
            /* read arr starting and end index */
            read(to_chil_1[0], &start, sizeof(start));
            read(to_chil_1[0], &end, sizeof(end));
            printf("\nc1: s: %d, e: %d\n",start,end);
            
            distributed_search(arr,start,end,val); /* function call to search for element */
            
        }
        else                /* parent process */
        {
           
            //close(to_chil_1[0]);                      /* close read end to child 1 */
            
            /* write arr starting and end index */
            write(to_chil_1[1], &start1, sizeof(start1));
            write(to_chil_1[1], &end1, sizeof(end1));
            printf("\ns1: %d, e1: %d\n",start1,end1);
        
            printf("Inside parent\n");
  
            pid = fork();
    
            if(pid < 0)
            {
                printf("Error creating process!\n");
                exit(1);
            }
            
            if(pid == 0) /* child 2 created here */
            {
                pid2 = getpid();   /* child 2 created */
                printf("Inside child 2\n");
        
                //close(to_chil_2[1]);                       /* close write end to child */
        
                int start, end;
        
                /* read arr starting and end index */
                read(to_chil_2[0], &start, sizeof(start));
                read(to_chil_2[0], &end, sizeof(end));
        
                printf("\nc2: s: %d, e: %d\n",start,end);
                distributed_search(arr,start,end,val); /* function call to search for element */
            }
            else
            {
              //  close(to_chil_2[0]);                      /* close read end to child 2 */
            
                write(to_chil_2[1], &start2, sizeof(start2));
                write(to_chil_2[1], &end2, sizeof(end2));
            
                printf("\ns2: %d, e2: %d\n",start2,end2);
                
                wait(NULL); /* Wait for child process to finish execution */
            }
            // wait(NULL);    /* check this out */
        }
        
    }
   else
    {
        printf("Parent searching\n");
        index = linear_search(arr,start,end,val);
        
        printf("\nSize here is: %d\n",size);
        printf("\nIndex here is: %d\n",index);
        
        if(index < 0 || index >= size)
        {
            return;
        }
        else
        {
            printf("in fn: %d\n",index);
            write(to_par[1], &index, sizeof(index));
            raise(1);
            return;
        }
    }
}

static void sig_usr(int signo)
{
    char buf[] = "signal caught!!\n";
    int wr = strlen(buf);
    write(STDOUT_FILENO, buf, wr);
    
    int index;
    read(to_par[0], &index, sizeof(index));
    
    printf("\nElement found at index : %d\n",index);
    
    kill(0,SIGTERM); //kill(0,SIGINT); //kill(0,SIGTERM);
    //exit(0);
    
}


int main(int argc, char *argv[])
{
    int val;
    setvbuf (stdout, NULL, _IONBF, 0);
    
    if(argc != 3)
    {
        printf("Incorrect arguments passed!\n");
        exit(1);
    }

    /* Calling function to create global array using the input file. */
    getarr(argv[1]);
    /* Converting number to be searched to integer. */
    val = atoi(argv[2]);
    printf("\n\nTo search: %d\n",val);
    
    signal(1,sig_usr);
    
    pipe(to_chil_1);    
    pipe(to_chil_2);
    pipe(to_par);
   
    
    /* function to search */
    int index = -1;

    distributed_search(arr, 0, size-1, val);
    printf("\nElement not found!\n");
    
    return 0;
}
