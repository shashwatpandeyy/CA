#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>

#define MIN_PID 300
#define MAX_PID 5000

int threadVar = 0;
pthread_mutex_t mutex;

struct pid_tab
{
    int pid;
    bool bitmap;
}pidArr[4700];

int allocate_map(void)
{
    int i,j;
    for(i = MIN_PID, j =0; i <= MAX_PID; i++, j++)
    {
        pidArr[j].pid = i;
        pidArr[j].bitmap = 0;
    }

    if(i == MAX_PID && j == 4700)
    return 1;

    else
    return -1;
}

int allocate_pid(void) 
{
    int i,j ;
	for( i = MIN_PID, j =0; i <= MAX_PID; i++, j++)
    {
        if(pidArr[j].bitmap == 0)
        {
            pidArr[j].pid = i;
            pidArr[j].bitmap = 1;
            return i;
            break;
        }
    }

    return -1;
}

void release_pid(int pid)
{
    int i;
	for( i = 0; i <= 4700; i++)
    {
        if(pidArr[i].pid == pid)
        {
            pidArr[i].bitmap = 0;
        }
    }
}


void *threadCall()
{
    int ret = allocate_pid();  
    while (threadVar < 100)
    {
        pthread_mutex_lock(&mutex);     
        if (threadVar >= 100)
        {
            pthread_mutex_unlock(&mutex);
            break;
        }

        threadVar++;                    
        sleep(0.001);
        printf("\n%d",threadVar);
        printf("%d\n",pidArr[threadVar].pid);
        pthread_mutex_unlock(&mutex);     
    }
    sleep(0.001);
    release_pid(ret);     
}

int main()
{
    int i =0;

    pthread_t thread[100];
    printf("\n 100 threads created.Every thread will print the value of variable 'threadVar' and increment it by 1 with a delay of 0.001s each process execute");
    sleep(5);    
    for(i = 0; i < 100; i++)
    {
        pthread_mutex_init(&mutex, NULL);
        pthread_create(&thread[i], NULL, threadCall, NULL);
        threadCall();
    }

    for(i = 0; i < 100; i++)
    {
        pthread_join(thread[i], NULL);
        pthread_mutex_destroy(&mutex);
    }

    return 0;
}
