/* 
 * The first line of the array file is assumed to contain the number of elements in the array.
 * 
 */

#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include<string.h>

int *arr, size;
int IN = -1;

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
    
    // Printing array
    printf("Size: %d\n",size);
    for(i=0; i<size; i++)
    {
        printf("%d  ",arr[i]);
    }
    
    
    fclose(fp);
    
}

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

    getarr(argv[1]);
    val = atoi(argv[2]);


    int pid;
    pid = fork();
    
    if(pid < 0)
    {
        printf("Error creating process!\n");
        exit(1);
    }
    
    if(pid == 0) //child
    {
        int index = linearSearch(arr,size,val);
        if(index == -1)
            exit(getpid());         // process exits by itself
        
        IN = index;            
    }
    else        //parent
    {
        
    }
    
    /*
    int index = linearSearch(arr,size,val);
    
    if(index == -1)
        printf("Number not found!\n");
    else
        printf("Element found at index %d.\n",index);

    */
    
    return 0;
}
