#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
// #include <math.h>
#include <unistd.h>

//global variables
long sum = 0;
long primeCounter = 0;
int numOfRandomNumbers;
int numT;
pthread_mutex_t mut;
int *randomNum;

int isPrime(int num)
{
	if (num <= 1)
		return 0;
	if (num <= 3)
		return 1;
	if (num % 2 == 0 || num % 3 == 0)
		return 0;

	for (int i = 5; i*i <= num; i = i + 6)
	{
		if (num % i == 0 || num % (i + 2) == 0)
			return 0;
	}
	return 1;
}

void *splitTheJob(void *tid)
{
	long tempSum = 0;
	long tempPcounter = 0;
	int *num = (int *)tid;
	int min = ((*num) * (numOfRandomNumbers / numT));
	int max = ((*num + 1) * (numOfRandomNumbers / numT));
	
	for (int j = min; j < max; j++){
		if (isPrime(randomNum[j])){
			tempSum += randomNum[j];
			tempPcounter++;
		}
	}
	pthread_mutex_lock(&mut);
	sum += tempSum;
	primeCounter += tempPcounter;
	pthread_mutex_unlock(&mut);
	return NULL;
}

int main(int argc, char *argv[])
{

	if (argc != 3)
	{
		printf("Too few arguments ");
		printf("USAGE: ./primeCalc <prime pivot> <num of random numbers>");
		exit(0);
	}

	int randomPivot = atoi(argv[1]);
	numOfRandomNumbers = atoi(argv[2]);
	numT = sysconf(_SC_NPROCESSORS_ONLN);
	randomNum = malloc(numOfRandomNumbers*sizeof(int));

	pthread_t pthreads[numT];

	srand(randomPivot);
	//generate random numbers
	for (int i = 0; i < numOfRandomNumbers; i++)
	{
		randomNum[i] = rand();;
	}

	for (int i = 0; i < numT; i++)
	{
		pthread_create(&pthreads[i], NULL, splitTheJob, &i);
		pthread_join(pthreads[i], NULL);
	}

	//keep the out format as this!!
	printf("%ld,%ld\n", sum, primeCounter);
	pthread_exit(NULL);

	exit(0);
}
