#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char text[10];
int s = 0;
pthread_rwlock_t rwl;
pthread_mutex_t m;
void* f(void* a)
{
	pthread_mutex_lock(&m);
	s += 1;
	pthread_mutex_unlock(&m);

	return NULL;
}

void* reader(void* a)
{
	pthread_rwlock_rdlock(&rwl);
	strcpy(text, a);
	pthread_rwlock_unlock(&rwl);
	
	pthread_rwlock_wrlock(&rwl);
	printf("%s \n", text);
	pthread_rwlock_unlock(&rwl);
	return NULL;
}
int main(int argc, char** argv)
{
	pthread_t t[100];
	int n = atoi(argv[2]);

	int i;
	pthread_mutex_init(&m,NULL);
	for(i=0; i<n; i++)
		pthread_create(&t[i],NULL,f,NULL);
	for(i=0; i<n; i++)
		pthread_join(t[i],NULL);
	pthread_mutex_destroy(&m);
	
	printf("%d \n", s);
	pthread_rwlock_init(&rwl,NULL);
	for(i=0; i<10*n; i++)
		pthread_create(&t[i],NULL,reader,argv[1]);
	for(i=0; i<10*n; i++)
		pthread_join(t[i],NULL);
	pthread_rwlock_destroy(&rwl);
	
	return 0;
}
