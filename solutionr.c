#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

int *randArr;
int numOfRandomNumbers;
int numOfThread;
long sum = 0;
long primeCounter = 0;
int idT=0;
pthread_mutex_t mutex;

bool isPrime(int n)
{
    if (n == 2 || n == 3)
    {
        return true;
    }
    else if (n <= 1 || n % 2 == 0 || n % 3 == 0)
    {
        return false;
    }

    int i = 5;
    while ((i * i) <= n)
    {
        if (n % i == 0 || n % (i + 2) == 0)
        {
            return false;
        }
        i += 6;
    }
    return true;
}

void *sum_prime()
{
    for (int i = idT * (numOfRandomNumbers / numOfThread); i < (idT + 1) * (numOfRandomNumbers / numOfThread); i++)
    {
        if (isPrime(randArr[i]))
        {
            //syncoronize//
            pthread_mutex_lock(&mutex);
            sum += randArr[i];
            primeCounter++;
            pthread_mutex_unlock(&mutex);
        }
    }
}

void create_threads(int numOfThreads)
{
    pthread_t threads[numOfThreads];

    int rc;

    for (int t = 0; t < numOfThreads; t++)
    {
        idT = t;
        /* Creates threads */
        rc = pthread_create(&threads[t], NULL, sum_prime, NULL);
        if (rc)
        {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
        pthread_join(threads[t], NULL);
    }
}

int main(int argc, char *argv[])
{
    numOfThread = sysconf(_SC_NPROCESSORS_ONLN);
    if (argc != 3)
    {
        printf("Too few arguments ");
        printf("USAGE: ./primeCalc <prime pivot> <num of random numbers>");
        exit(0);
    }

    int seed = atoi(argv[1]);       //~atoi- converts the string argument to an integer~//
    numOfRandomNumbers = atoi(argv[2]);

    //init rundom generator
    srand(seed);        //~srand- seeds the random number generator used by the function rand~//

    //generate random numbers to array
    randArr = malloc(numOfRandomNumbers * sizeof(int));
    for (int i = 0; i < numOfRandomNumbers; i++)
    {
        randArr[i] = rand();
    }

    create_threads(numOfThread);

    //keep the out format as this!!
    printf("%ld,%ld\n", sum, primeCounter);

    /* Exits the threads */
    pthread_exit(NULL);

    return 0;
}