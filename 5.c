#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <string.h>

int main(int argc, char** argv)
{
	int a2c[2],c2b[2],b2a[2];
	
	int dim = 1;
	char s[16];
	s[0] = '\0';
	printf("A: \n");

	pipe(a2c);
	pipe(c2b);
	pipe(b2a);

	if(fork()==0)
	{/* Procesul C */
		close(b2a[0]);close(b2a[1]);
		close(a2c[1]);close(c2b[0]);

		while(1)
		{
			if(read(a2c[0], &dim, sizeof(int))<=0)
				break;

			read(a2c[0], s, dim);
			
			if(dim>15)
				break;

			printf("C: %s -> +c \n", s);
			strcat(s,"c");
			dim++;

			write(c2b[1], &dim, sizeof(int));
			write(c2b[1], s, dim);
		}

		close(a2c[0]);close(c2b[1]);
		exit(0);
	}

	if(fork()==0)
	{/* Procesul B */
		close(a2c[0]);close(a2c[1]);
		close(c2b[1]);close(b2a[0]);

		while(1)
		{
			if(read(c2b[0], &dim, sizeof(int))<=0)
				break;

			read(c2b[0], s, dim);

			if(dim>15)
				break;
			printf("B: %s -> +b \n", s);
			strcat(s,"b");
			dim++;

			write(b2a[1], &dim, sizeof(int));
			write(b2a[1], s, dim);
		}
		
		close(c2b[0]);close(b2a[1]);
		exit(0);
	}

	close(c2b[0]);close(c2b[1]);
	close(b2a[1]);close(a2c[0]);

	write(a2c[1], &dim, sizeof(int));
	write(a2c[1], s, dim);
	while(1)
	{
		if(read(b2a[0], &dim, sizeof(int))<=0)
			break;
		read(b2a[0], s, dim);

		if(dim>15)
			break;

		if(dim%2==1)
		{
			printf("A: %s -> +A \n", s);
			strcat(s, "A");
			dim++;
		}
		else
		{
			printf("A: %s -> +a \n", s);
			strcat(s, "a");
			dim++;
		}
		write(a2c[1], &dim, sizeof(int));
		write(a2c[1], s, dim);
	}
	close(b2a[0]);
	close(a2c[1]);
	wait(0);
	wait(0);

	return 0;
}
