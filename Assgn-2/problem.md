Problem Statement 2 - Delegated Linear Search  

Objective: Implementation of Distributed Linear Search using Linux System calls - An array is
split up into sub-arrays and searching the sub-arrays for an integer is delegated to other processes.
Input: The program should take two arguments as input - an array of integers to be read from a
file (first argument is the path to the file) and an integer to be searched (second argument).  

Output: Print index of the number to be searched if found, or ‘Number not found’.  

Method:  
The main program reads the complete input array as a global array. The main program creates 2
processes and gives one half of the input array to the 2 child processes to search the required
number. If the size of the segment is small enough (say <= 5), then it searches the number in the
segment by itself. Otherwise, the task is delegated by creating more child processes wherein each
process is given a segment of the array to handle and the number to find.
If any process finds the required number, then it sends a signal to the main process and also returns
the index. After this, the main process kills all the other processes and prints the index. The code
should also handle the case when the number is not present in the array.
Assume PIPE as the chosen method of IPC mechanism.
