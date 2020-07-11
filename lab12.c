#include <stdio.h>
#include <pthread.h>

int s =0;
pthread_mutex_t m;

void* f(void* a)
{
	pthread_mutex_lock(&m);
	s = s + 1;
	pthread_mutex_unlock(&m);

	return NULL;
}

int charToInt(char* x)
{
	int i=0;
	int value = 0;
	while(x[i]!=NULL)
	{
		value = value*10 + (x[i]-'0');
		i++;
	}

	return value;
}

int main(int argc, char** argv)
{
	int n = charToInt(argv[1]);
	pthread_t t[n];

	int i;
	pthread_mutex_init(&m,NULL);

	for(i=0; i<n; i++)
		pthread_create(&t[i],NULL,f,NULL);

	for(i=0; i<n; i++)
		pthread_join(t[i],NULL);

	pthread_mutex_destroy(&m);

	printf("%d \n", s);

	return 0;
}

