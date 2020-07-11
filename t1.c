#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

int sum = 0;
pthread_mutex_t m;


int factorial(int x)
{
	int i;
	int rez = 1;
	for(i=2; i<=x; i++)
		rez = rez* i;

	return rez;
}
int prime(int x)
{
	if(x==0)
		return 0;

	if(x%2==0 && x!=2)
		return 0;

	int div;
	for(div=3; div*div<=x; div+=2)
		if(x%div==0)
			return 0;

	return 1;
}

int charToInt(char* x)
{
	int val = 0;
	int i=0;
	while(x[i] != NULL)
	{
		val = val*10 + (x[i] -'0');
		i++;
	}
	return val;
}

void* fa(void* a)
{	
	printf("%d! = %d \n", *(int*)a, factorial(*(int*)a));
	return NULL;
}

void* fb(void* a)
{
	if(prime(*(int*)a)!=0)
	{
		pthread_mutex_lock(&m);
		sum = sum + *(int*)a;
		pthread_mutex_unlock(&m);
	}
	return NULL;
}

int main(int argc, char** argv)
{
	pthread_t t[100];
	int i;
	int valori[20] = {0};

	for(i=0; i<argc-1; i++)
		valori[i] = charToInt(argv[i+1]);

	pthread_mutex_init(&m,NULL);
	int k=0;
	for(i=0; i<(argc-1)*2; i+=2)
	{
		pthread_create(&t[i],NULL,fa,&valori[k]);
		pthread_create(&t[i+1],NULL,fb,&valori[k]);
		k++;
	}
	
	for(i=0; i<(argc-1)*2; i++)
		pthread_join(t[i],NULL);
	pthread_mutex_destroy(&m);
	printf("Sum of prime numbers: %d \n", sum);
	return 0;
}
