#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

char string[]="hello";
int main()
{
	int fd2;
	char buff[1024];


		mknod("fifo1",010777,0);

		fd2 = open("fifo1",O_RDONLY);
		
		int count=0;
		//for(;;)
		do		
		{
		
			read(fd2, buff, 1024);
			printf("output: %d%s\n",count++,buff);
		}
		while(1);
		
		close(fd2);
		return 0;
}
