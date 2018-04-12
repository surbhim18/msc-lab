/*
 * Implementation of Distributed Linear Search using Linux System calls
 * An array is split up into sub-arrays and searching the sub-arrays for an integer is delegated to other processes.
 */

#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>

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
int linearSearch(int *arr, int size, int val)
{
    int i;
    for(i=0; i<size; i++)
    {
        if(arr[i] == val)
            return i;
    }
    
    return -1;
}

int main(int argc, char *argv[])
{
    int val;
    
    if(argc != 3)
    {
        printf("Incorrect arguments passed!\n");
        exit(1);
    }

    /* Calling function to create global array using the input file. */
    getarr(argv[1]);
    /* Converting number to be searched to integer. */
    val = atoi(argv[2]);

    /* Pipe for IPC */
    int to_par[2];
    pipe(to_par);

    /* Creating a child process */
    pid_t pid;
    pid = fork();
    
    if(pid < 0)
    {
        printf("Error creating process!\n");
        exit(1);
    }

    //fork children till array size <= 5 for each process 
    if(pid == 0) /* Child process */
    {
        printf("Inside child\n");
        
        int index = linearSearch(arr,size,val); /* function call to search for element */
        if(index == -1)
            exit(getpid());         /* process exits by itself if element not found */
            
        
        close(to_par[0]);                        /* close read end of pipe */
        write(to_par[1], &index, sizeof(index)); /* writing to pipe */
        
    }
    else      /* Parent process */
    {
        wait(); /* Wait for child process to finish execution */
        
        printf("Inside parent\n");
  
        close(to_par[1]);                       /* close write end of pipe */
        int index = -1;
        read(to_par[0], &index, sizeof(index)); /* reading from pipe */
    
        if(index == -1)
            printf("Number not found!\n");
        else
            printf("Element found at index %d.\n",index);
        
        //parent process should recieve the signal
        //parent process shouldn't have to wait for all processes to complete execution
        //kill all other processes
        //handle not getting any signal
    }
    
    return 0;
}
