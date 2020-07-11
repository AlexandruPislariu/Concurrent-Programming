#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

int main(int argc, char** argv)
{
	int p2c[2],c2p[2];
	int N;

	scanf("%d", &N);
	printf("P: %d \n", N);

	pipe(p2c);
	pipe(c2p);
	if(fork()==0)
	{/* Procesul copil C */
		close(p2c[1]);
		close(c2p[0]);
		
		while(1)
		{
			if(read(p2c[0], &N, sizeof(int)) <= 0)
				break;

			if(N<=5)
				break;

			printf("C: %d -> %d \n", N, N-2);
			N = N-2;
			write(c2p[1], &N, sizeof(int));
		}

		close(p2c[0]);
		close(c2p[1]);

		exit(0);
	}
	
	close(p2c[0]);
	close(c2p[1]);

	write(p2c[1], &N, sizeof(int));
	while(1)
	{
		if(read(c2p[0], &N, sizeof(int)) <=0 )
			break;

		if(N<=5)
			break;

		if(N%2==0)
		{	
			printf("P: %d -> %d \n", N, N-2);
			N = N-2;
		}
		else
		{
			printf("P: %d -> %d \n", N, N-1);
			N = N-1;
		}
		write(p2c[1], &N, sizeof(int));
	}
	close(c2p[0]);
	close(p2c[1]);
	wait(0);

	mkfifo("/tmp/fifo1", 0666);
	int file = open("/tmp/fifo1", O_WRONLY);
	write(file, &N, sizeof(int));
	close(file);

	return 0;
}
