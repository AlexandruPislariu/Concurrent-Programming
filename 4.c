#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

int main(int argc, char** argv)
{
	int a2c,c2b,b2a;

//	unlink("/tmp/a2c");
//	unlink("/tmp/c2b");
//	unlink("/tmp/b2a");

	mkfifo("/tmp/a2c", 0777);
	mkfifo("/tmp/c2b", 0777);
	mkfifo("/tmp/b2a", 0777);

	int N;
	scanf("%d", &N);
	printf("A: %d \n", N);

	if(fork()==0)
	{/* Procesul C */
		a2c = open("/tmp/a2c", O_RDONLY);
		c2b = open("/tmp/c2b", O_WRONLY);
		
		while(1)
		{	
			if(read(a2c, &N, sizeof(int))<=0)
				break;

			if(N<=0)
				break;
			
			printf("C: %d -> %d \n", N, N+1);
			N++;

			write(c2b, &N, sizeof(int));
		}
		close(a2c);
		close(c2b);
		exit(0);
	}
	if(fork()==0)
	{/* Procesul B */
		c2b = open("/tmp/c2b", O_RDONLY);
		b2a = open("/tmp/b2a", O_WRONLY);

		while(1)
		{
			if(read(c2b, &N, sizeof(int))<=0)
				break;

			if(N<=0)
				break;

			printf("B: %d -> %d \n", N, N-2);
			N = N- 2;

			write(b2a, &N, sizeof(int));
		}
		close(c2b);
		close(b2a);
		exit(0);
	}

	a2c = open("/tmp/a2c", O_WRONLY);
	b2a = open("/tmp/b2a", O_RDONLY);

	write(a2c, &N, sizeof(int));
	while(1)
	{	
		if(read(b2a, &N, sizeof(int))<=0)
			break;

		if(N<=0)
			break;

		if(N%2==0)
		{
			printf("A: %d -> %d \n", N, N-2);
			N = N-2;
		}
		else
		{
			printf("A: %d -> %d \n", N, N-1);
			N = N-1;
		}
		write(a2c, &N, sizeof(int));
	}
	close(a2c);
	close(b2a);
	
	wait(0);
	wait(0);

	unlink("/tmp/a2c");
	unlink("/tmp/c2b");
	unlink("/tmp/b2a");

	return 0;
}

