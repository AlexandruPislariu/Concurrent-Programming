#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

void f(int sgn)
{	
	sleep(5);
}

int main(int argc,char** argv)
{	
	int pid = fork();
	if(pid == 0)
	{
		execlp("grep","grep","/gr911","./passwd.txt",NULL);
		exit(0);
	}

	wait(0);
	return 0;
}
