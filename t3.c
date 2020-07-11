#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <pthread.h>

int sum_fact=0,sum_patr=0;
pthread_mutex_t mF,mP;

int factorial(int x)
{
	int f = 1;
	int i;
	for(i=2; i<=x; i++)
		f*=i;

	return f;
}
void* fa(void* a)
{
	int value = *(int*)a;
	int fact = factorial(value);
	pthread_mutex_lock(&mF);
	sum_fact += fact;
	pthread_mutex_unlock(&mF);
	
	return NULL;
}
void* fb(void* a)
{
	int value = *(int*)a;
	pthread_mutex_lock(&mP);
	sum_patr += value*value;
	pthread_mutex_unlock(&mP);

	return NULL;
}

int main(int argc, char** argv)
{
	int valori[100];
	int i;
	for(i=1; i<argc; i++)
		valori[i-1] = atoi(argv[i]);

	pthread_t t[200];
	int a2b[2],b2p[2];
	pipe(a2b);pipe(b2p); 

	if(fork()==0)
	{/* Procesul A */
		close(a2b[0]);close(b2p[0]);
		close(b2p[1]);
		
		pthread_mutex_init(&mF,NULL);
		int k=0;
		for(i=0; i<argc-1; i++)
			if(valori[i]%2==0)
			{
				pthread_create(&t[k],NULL,fa,&valori[i]);
				k++;
			}
		for(i=0; i<k; i++)
			pthread_join(t[i],NULL);

		pthread_mutex_destroy(&mF);
		write(a2b[1], &sum_fact, sizeof(int));
		close(a2b[1]);
		exit(0);
	}
	if(fork()==0)
	{/* Procesul B */
		close(a2b[1]);close(b2p[0]);
		
		read(a2b[0], &sum_patr, sizeof(int));

		pthread_mutex_init(&mP, NULL);
		int k=0;
		for(i=0; i<argc-1; i++)
			if(valori[i]%2==1)
			{
				pthread_create(&t[k], NULL,fb,&valori[i]);
				k++;
			}
		for(i=0; i<k; i++)
			pthread_join(t[i],NULL);

		pthread_mutex_destroy(&mP);
		write(b2p[1], &sum_patr, sizeof(int));
		exit(0);
	}
	
	close(a2b[0]);close(a2b[1]);
	close(b2p[1]);
	int sum_final;
	read(b2p[0], &sum_final, sizeof(int));
	printf("Sum: %d\n", sum_final);

	wait(0);
	wait(0);

	return 0;
}

