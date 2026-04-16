#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

int thread_count;

void *Hello(void *rank);

int main(int argc, char *argv[])
{
    long thread;
    pthread_t* thread_handles;

    thread_count = strtol(argv[1], NULL, 10);

    thread_handles = malloc(thread_count * sizeof(pthread_t));

    for(thread = 0; thread < thread_count; thread++)
    {
        pthread_create(&thread_handles[thread],
                       NULL,
                       Hello,
                       (void*) thread);
    }

    printf("Hello from the main thread\n");

    for(thread = 0; thread < thread_count; thread++)
    {
        pthread_join(thread_handles[thread], NULL);
    }

    free(thread_handles);
    return 0;
}

void *Hello(void *rank)
{
    struct timeval start, end;
    gettimeofday(&start, NULL);

    long my_rank = (long) rank;

    printf("Hello from thread %ld of %d\n",
           my_rank, thread_count);

    gettimeofday(&end, NULL);

    double time_taken =
        (end.tv_sec - start.tv_sec) +
        (end.tv_usec - start.tv_usec) / 1000000.0;

    printf("Thread %ld Execution Time: %f seconds\n",
           my_rank, time_taken);

    return NULL;
}
