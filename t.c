#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void* f(void* a)
{
	printf("%d\n", *(int*)a);
	return NULL;
}

int main(int argc, char** argv)
{
	pthread_t t[11];
	int i;
	for(i=0; i<10; i++)
	{
		pthread_create(&t[i],NULL,f,&i);
	}

	for(i=0; i<10; i++)
		pthread_join(t[i],NULL);
}
