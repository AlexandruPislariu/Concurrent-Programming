#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

int main(int argc, char** argv)
{
	int file = open("/tmp/fifo2", O_RDONLY);
	int dim;
	char s[16];
	read(file, &dim, sizeof(int));
	read(file, s, dim*sizeof(char));
	close(file);

	printf("STRING: %s \n", s);

	return 0;
}
