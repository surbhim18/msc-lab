/*
 * Roll no: 43, 46
 * Surbhi Mittal, Thongminlien Kuki
 * 
 * Implementation of Distributed Linear Search using Linux System calls.
 * An array is split up into sub-arrays and searching the sub-arrays for an integer is delegated to other processes.
 * 
 * Execute as: ./a.out array.txt 24
 * 
 * Format of array.txt:
 * First line contains the number of elements in the array.
 * Each line contains one element, thereafter.
 * 
 */

#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* global declaration of array and array size. */
int *arr, size;



/* 
 * Function to fill an array using file data. 
 */
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
    size = atoi(str);  /* converting character data to integer */
    
    int i;
    arr = (int*)malloc(size*sizeof(int));   /* allocating memory for array */
    
    for(i=0; i<size; i++)
    {
        fgets(str, sizeof(str), fp);
        arr[i] = atoi(str);
    }
    
    fclose(fp);
    
}

/*
 * Function for linear search; returns index of element, if found 
 */
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

/* For pipes - GLOBAL */
int to_chil_1[2];  /* information to child process 1 */
int to_chil_2[2];  /* information to child process 2 */
int to_par[2];     /* information to parent */


/*
 * Function creates 2 child processes if array size greater than 5,
 * else, searches for the element itself.
 */
void distributed_search(int *arr, int start, int end, int val)
{
    
    int index;
    int arrsize = end - start +1;
    
     if(start < 0 || end >= size || start > end)
        exit(0);
    
     if(arrsize > 5)    /* array size > 5 */
     {
         
        pid_t pid1, pid2;
        
        /* calculating start and end indices for child processes */
        int start1 = start; int end1 = start + (arrsize/2)-1;
        int start2 = start + (arrsize/2); int end2 = end;
        
        /* creating first child process */
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
            int start, end;
        
            /* reading array start and end index from child 1's pipe */
            read(to_chil_1[0], &start, sizeof(start));
            read(to_chil_1[0], &end, sizeof(end));
            
            distributed_search(arr,start,end,val); /* recursive call to search for element */
            
        }
        else                /* parent process */
        {
            /* writing array start and end index to child 1's pipe */
            write(to_chil_1[1], &start1, sizeof(start1));
            write(to_chil_1[1], &end1, sizeof(end1));
  
            /* creating second child process */
            pid = fork();
    
            if(pid < 0)
            {
                printf("Error creating process!\n");
                exit(1);
            }
            
            if(pid == 0) /* child 2 created */
            {
                pid2 = getpid();   
                int start, end;
        
                /* reading array start and end index from child 2's pipe */
                read(to_chil_2[0], &start, sizeof(start));
                read(to_chil_2[0], &end, sizeof(end));
        
                distributed_search(arr,start,end,val); /* recursive call to search for element */
            }
            else
            {
                /* writing array start and end index to child 1's pipe */
                write(to_chil_2[1], &start2, sizeof(start2));
                write(to_chil_2[1], &end2, sizeof(end2));
                
                 /* wait for child process to finish execution */
                wait(NULL);
            }
        }
    }
   else /* array size <= 5 */
    {
        /* search linearly in the array */
        index = linear_search(arr,start,end,val);
                
        if(index < 0 || index >= size)  /* invalid index */
        {
            exit(0);
        }
        else   /* valid index */
        {
            /* write to parent pipe */
            write(to_par[1], &index, sizeof(index));
            
            /* raise signal */
            raise(1);
            exit(0);
        }
    }
}


/* 
 * signal handler -
 * prints index of element found and kills all related processes.
 */
static void sig_usr(int signo)
{
    int index;
    /* read from parent pipe */
    read(to_par[0], &index, sizeof(index));
    
    printf("Element found at index : %d\n",index);

    /* terminate all processes in the same group i.e., all child processes spawned*/
    kill(0,SIGTERM); 

    exit(0);    
}


int main(int argc, char *argv[])
{
    int val;
    
    /* flush buffer */
    setvbuf (stdout, NULL, _IONBF, 0);
    
    if(argc != 3)
    {
        printf("Incorrect arguments passed!\n");
        exit(1);
    }

    /* calling function to create global array using the input file. */
    getarr(argv[1]);
    
    /* converting number to be searched to integer. */
    val = atoi(argv[2]);
    
    /* declare user defined signal */
    signal(1,sig_usr);
    
    /* create pipes for communication */
    pipe(to_chil_1);    
    pipe(to_chil_2);
    pipe(to_par);
   
    int index;

    /* function call to search */
    distributed_search(arr, 0, size-1, val);
    
    /* wait for child processes to finish */
    wait(NULL);
    
    printf("Element not found\n");
    
    kill(0,SIGTERM); 
    
    return 0;
}
