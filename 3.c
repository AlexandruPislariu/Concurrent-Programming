#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

int main(int argc, char** argv)
{
	int a2c[2],c2b[2],b2a[2];
	int N;
	scanf("%d", &N);
	printf("A: %d \n", N);

	pipe(a2c);pipe(c2b);pipe(b2a);

	if(fork()==0)
	{/* Procesul C */
		close(b2a[0]);close(b2a[1]);
		close(a2c[1]);close(c2b[0]);

		while(1)
		{
			if(read(a2c[0], &N, sizeof(int)) <= 0)
				break;

			if(N<=0)
				break;

			printf("C: %d -> %d \n", N, N+1);
			N++;

			write(c2b[1], &N, sizeof(int));
		}
		close(a2c[0]);
		close(c2b[1]);
		exit(0);
	}

	if(fork()==0)
	{/* Procesul B */
		close(a2c[0]);close(a2c[1]);
		close(c2b[1]);close(b2a[0]);

		while(1)
		{
			if(read(c2b[0], &N, sizeof(int)) <=0)
				break;

			if(N<=0)
				break;

			printf("B: %d -> %d \n", N, N-2);
			N = N - 2;

			write(b2a[1], &N, sizeof(int));
		}

		close(c2b[0]);close(b2a[1]);
		exit(0);
	}
	close(c2b[0]);close(c2b[1]);
	close(b2a[1]);close(a2c[0]);
	
	write(a2c[1], &N, sizeof(int));
	while(1)
	{
		if(read(b2a[0], &N, sizeof(int))<=0)
			break;

		if(N<=0)
			break;

		if(N%2==0)
		{
			printf("A: %d -> %d \n", N, N-2);
			N = N - 2;
		}
		else
		{
			printf("A: %d -> %d \n", N, N-1);
			N = N - 1;
		}
		write(a2c[1], &N, sizeof(int));
	}
	close(a2c[1]);close(b2a[0]);
	wait(0);
	wait(0);
	return 0;
}
