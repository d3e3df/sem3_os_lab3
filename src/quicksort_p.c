#include "quicksort_p.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

sem_t sem;
int total_threads_created = 0;

void quicksort_sequential(int* arr, int left, int right) {
    if (left >= right)
        return;
    int i = left, j = right;
    int pivot = arr[(left + right) / 2];
    while (i <= j) {
        while (arr[i] < pivot)
            i++;
        while (arr[j] > pivot)
            j--;
        if (i <= j) {
            int tmp = arr[i];
            arr[i] = arr[j];
            arr[j] = tmp;
            i++;
            j--;
        }
    }
    quicksort_sequential(arr, left, j);
    quicksort_sequential(arr, i, right);
}

static void* thread_func(void* arg) {
    ThreadArg* targ = (ThreadArg*)arg;
    quicksort_parallel(targ->arr, targ->left, targ->right);
    free(targ);
    sem_post(&sem);
    return NULL;
}

void quicksort_parallel(int* arr, int left, int right) {
    if (left >= right)
        return;

    if ((right - left) < THRESHOLD) {
        quicksort_sequential(arr, left, right);
        return;
    }

    int i = left, j = right;
    int pivot = arr[(left + right) / 2];
    while (i <= j) {
        while (arr[i] < pivot)
            i++;
        while (arr[j] > pivot)
            j--;
        if (i <= j) {
            int tmp = arr[i];
            arr[i] = arr[j];
            arr[j] = tmp;
            i++;
            j--;
        }
    }

    if (sem_trywait(&sem) == 0) {
        ThreadArg* arg = malloc(sizeof(ThreadArg));
        if (!arg) {
            perror("malloc");
            return;
        }
        arg->arr = arr;
        arg->left = left;
        arg->right = j;

        pthread_t tid;
        if (pthread_create(&tid, NULL, thread_func, arg) != 0) {
            perror("pthread_create");
            free(arg);
            sem_post(&sem);
            quicksort_sequential(arr, left, j);
            quicksort_parallel(arr, i, right);
            return;
        }

        total_threads_created++;
        quicksort_parallel(arr, i, right);
        pthread_join(tid, NULL);
    } else {
        quicksort_sequential(arr, left, j);
        quicksort_sequential(arr, i, right);
    }
}