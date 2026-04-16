#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

#define MAX 1000

int arr[MAX];
int n, thread_count;
long long global_sum = 0;

pthread_mutex_t mutex;

void *partial_sum(void *rank)
{
    struct timeval start, end;
    gettimeofday(&start, NULL);

    long my_rank = (long) rank;

    int local_n = n / thread_count;
    int first = my_rank * local_n;
    int last = first + local_n - 1;

    long long local_sum = 0;

    printf("Thread %ld: First index = %d, Last index = %d\n",
           my_rank, first, last);

    for(int i = first; i <= last; i++)
    {
        local_sum += arr[i];
    }

    pthread_mutex_lock(&mutex);
    global_sum += local_sum;
    pthread_mutex_unlock(&mutex);

    gettimeofday(&end, NULL);

    double time_taken =
        (end.tv_sec - start.tv_sec) +
        (end.tv_usec - start.tv_usec) / 1000000.0;

    printf("Thread %ld Execution Time: %f seconds\n", my_rank, time_taken);

    pthread_exit(NULL);
}

int main()
{
    pthread_t *thread_handles;

    printf("Enter number of elements: ");
    scanf("%d", &n);

    printf("Enter the elements:\n");
    for(int i = 0; i < n; i++)
        scanf("%d", &arr[i]);

    printf("Enter number of threads: ");
    scanf("%d", &thread_count);

    thread_handles = malloc(thread_count * sizeof(pthread_t));

    pthread_mutex_init(&mutex, NULL);

    for(long thread = 0; thread < thread_count; thread++)
    {
        pthread_create(&thread_handles[thread],
                       NULL,
                       partial_sum,
                       (void*) thread);
    }

    for(long thread = 0; thread < thread_count; thread++)
        pthread_join(thread_handles[thread], NULL);

    pthread_mutex_destroy(&mutex);

    printf("\nTotal Sum = %lld\n", global_sum);

    free(thread_handles);
    return 0;
}
