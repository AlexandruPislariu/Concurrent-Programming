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
	int p2c[2],c2p[2];
	int dim = 1;
	char s[16];
	s[0] = '\0';

	printf("P: \n");
	
	pipe(p2c);
	pipe(c2p);
	if(fork()==0)
	{/* Procesul C */
		close(p2c[1]);
		close(c2p[0]);

		while(1)
		{
			if(read(p2c[0], &dim, sizeof(int)) <=0)
				break;
			
			read(p2c[0], s, dim*sizeof(char));

			if(dim>14)
				break;

			printf("C: %s -> +c \n", s);
			strcat(s,"c");
			dim = dim + 1;
			write(c2p[1], &dim, sizeof(int));
			write(c2p[1], s, dim);
		}
		close(p2c[0]);
		close(c2p[1]);

		exit(0);
	}

	close(c2p[1]);
	close(p2c[0]);

	while(1)
	{	
		write(p2c[1], &dim, sizeof(int));
		write(p2c[1], s, dim);

		if(read(c2p[0], &dim, sizeof(int)) <=0 )
			break;

		read(c2p[0], s, dim);

		if(dim>14)
			break;

		if(dim %2 == 1)
		{
			printf("P: %s -> +PP \n", s);
		       	strcat(s, "PP");
	 		dim = dim + 2;
		}
		else
		{
			printf("P: %s -> +pp \n", s);
			strcat(s, "pp");
			dim = dim + 2;
		}
	}
	close(c2p[0]);
	close(p2c[1]);
	wait(0);
	
	unlink("/tmp/fifo2");
	mkfifo("/tmp/fifo2", 0666);
	int file = open("/tmp/fifo2", O_WRONLY);
	write(file, &dim, sizeof(int));
	write(file, s, dim);
	close(file);

	return 0;
}	
