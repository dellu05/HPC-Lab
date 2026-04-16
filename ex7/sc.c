#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define MAX 10
#define WORD 50
#define THREADS 3

char buffer[MAX][WORD];
char result[MAX][WORD];
char status[MAX][20];

int in = 0, out = 0, count = 0;
int total, processed = 0, taken = 0;

pthread_mutex_t mutex;
pthread_cond_t not_empty, not_full;

char dict[][WORD] = {"apple", "cat", "dog", "house", "tree", "book", "pen"};

int check(char *w) {
    for (int i = 0; i < 7; i++)
        if (strcmp(w, dict[i]) == 0)
            return 1;
    return 0;
}

void *spell_check(void *arg) {
    char word[WORD];
    while (1) {
        pthread_mutex_lock(&mutex);

        while (count == 0 && taken < total)
            pthread_cond_wait(&not_empty, &mutex);

        if (taken >= total && count == 0) {
            pthread_mutex_unlock(&mutex);
            break;
        }

        strcpy(word, buffer[out]);
        out = (out + 1) % MAX;
        count--;
        taken++;

        pthread_cond_signal(&not_full);
        pthread_mutex_unlock(&mutex);

        int ok = check(word);

        pthread_mutex_lock(&mutex);
        strcpy(result[processed], word);
        if (ok) strcpy(status[processed], "CORRECT");
        else strcpy(status[processed], "INCORRECT");
        processed++;
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main() {
    pthread_t t[THREADS];
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&not_empty, NULL);
    pthread_cond_init(&not_full, NULL);

    printf("Enter number of words: ");
    if (scanf("%d", &total) != 1) return 0;

    for (int i = 0; i < THREADS; i++)
        pthread_create(&t[i], NULL, spell_check, NULL);

    for (int i = 0; i < total; i++) {
        char word[WORD];
        printf("Enter word %d: ", i + 1);
        scanf("%s", word);

        pthread_mutex_lock(&mutex);
        while (count == MAX)
            pthread_cond_wait(&not_full, &mutex);

        strcpy(buffer[in], word);
        in = (in + 1) % MAX;
        count++;

        pthread_cond_signal(&not_empty);
        pthread_mutex_unlock(&mutex);
    }

    pthread_mutex_lock(&mutex);
    pthread_cond_broadcast(&not_empty);
    pthread_mutex_unlock(&mutex);

    for (int i = 0; i < THREADS; i++)
        pthread_join(t[i], NULL);

    printf("\nResults:\n");
    for (int i = 0; i < processed; i++)
        printf("%s -> %s\n", result[i], status[i]);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&not_empty);
    pthread_cond_destroy(&not_full);

    return 0;
}
