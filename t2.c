#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int sum_total = 0, sum = 0;
pthread_mutex_t mTotal, mPalindrom;

int charToInt(char* x)
{
	int value=0;
	int i=0;
	while( x[i] != NULL)
	{
		value = value*10 + (x[i]-'0');
		i++;
	}

	return value;
}

void* fa(void* a)
{
	int value = *(int*)a;
	int sum_value = (value)*(value+1)/2;
	printf("sum 0->%d  = %d\n", value,sum_value);

	pthread_mutex_lock(&mTotal);
	sum_total = sum_total + sum_value;
	pthread_mutex_unlock(&mTotal);

	return NULL;
}

int palindrome(int x)
{
	int reverse = 0;
	int cx = x;
	while(cx>0)
	{
		reverse = reverse*10 + cx%10;
		cx = cx/10;
	}

	return(x==reverse);
}

void* fb(void* a)
{
	int value = *(int*)a;
	if(value < 10)
	{	
		pthread_mutex_lock(&mPalindrom);
		sum = sum+value;
		pthread_mutex_unlock(&mPalindrom);
	}

	return NULL;
}

void* fc(void* a)
{
	int value = *(int*)a;
	if(palindrome(value)!=0)
	{
		pthread_mutex_lock(&mPalindrom);
		sum = sum + value;
		pthread_mutex_unlock(&mPalindrom);
	}

	return NULL;
}

int main(int argc, char** argv)
{
	pthread_t t[100];
	int valori[30];

/* Salvez valorile */
	int i;
	for(i=0; i<argc-1; i++)
		valori[i] = charToInt(argv[i+1]);

	pthread_mutex_init(&mTotal,NULL);
	pthread_mutex_init(&mPalindrom, NULL);
	int k=0;
	for(i=0; i<3*(argc-1); i+=3)
	{
		pthread_create(&t[i],NULL,fa,&valori[k]);
		pthread_create(&t[i+1],NULL,fb,&valori[k]);
		pthread_create(&t[i+2],NULL,fc,&valori[k]);
		k++;
	}

	for(i=0; i<3*(argc-1); i++)
		pthread_join(t[i],NULL);
	pthread_mutex_destroy(&mTotal);
	pthread_mutex_destroy(&mPalindrom);
	printf("Sum palidrom: %d \n", sum);
	printf("Sum total: %d \n", sum_total);

	return 0;
}
