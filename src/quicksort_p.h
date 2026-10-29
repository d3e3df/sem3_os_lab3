#ifndef QUICKSORT_P_H
#define QUICKSORT_P_H

#include <semaphore.h>

#define THRESHOLD 5000

typedef struct {
    int* arr;
    int left;
    int right;
} ThreadArg;

extern sem_t sem;
extern int total_threads_created;

void quicksort_parallel(int* arr, int left, int right);
void quicksort_sequential(int* arr, int left, int right);

#endif