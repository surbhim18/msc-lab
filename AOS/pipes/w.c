#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

char string[]="hello";
int main()
{
	int fd1,count=0;
	
		mknod("fifo1",010777,0);

		fd1 = open("fifo1",O_WRONLY);
		//printf("fjgcjk");
		//for(int i=0;i<5;i++)
		//for(int i=0;i<1;i++)		
		do		
		{
		
			write(fd1, string, 1024);
			printf("input: %d%s\n",count++,string);

		}
		while(1);
		close(fd1);

		return 0;
}

