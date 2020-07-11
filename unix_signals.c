#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>

void f(int sgn)
{	
	sleep(5);

	wait(0);
	
	char s[32];
	printf("Are you sure you want to exit process? ");
	scanf("%s",s);
	
	if(strcmp(s, "y") == 0)
	{	
		sleep(5);
		exit(0);
	}
}

int main(int argc,char** argv)
{
	signal(SIGCHLD, f);
	
	while(1)
	{
		int pid;
		pid = fork();
		if(pid == 0)
		{
			exit(0);
		}
	}
	return 0;
}
