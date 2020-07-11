#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

int main(int argc, char** argv)
{
	int file = open("/tmp/fifo1", O_RDONLY);
	int N;
	read(file, &N, sizeof(int));
	close(file);
	printf("RESULT: %d \n", N);

	return 0;
}
