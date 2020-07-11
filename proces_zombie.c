#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc,char** argv)
{
	int pid;
	pid = fork();
	if(pid == 0)
	{
		sleep(5);
		exit(0);
	}
	sleep(5);
	wait(0);
	sleep(5);
	return 0;
}

