#include "quicksort_p.h"
#include <getopt.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

static void fill_random(int* arr, int n) {
    for (int i = 0; i < n; i++)
        arr[i] = rand() % 1000000;
}

static int is_sorted(const int* arr, int n) {
    for (int i = 1; i < n; i++)
        if (arr[i - 1] > arr[i])
            return 0;
    return 1;
}

static double now_sec(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec / 1e9;
}

int main(int argc, char* argv[]) {
    int num_threads = 4;
    int size = 1000000;
    int opt;

    while ((opt = getopt(argc, argv, "t:s:")) != -1) {
        switch (opt) {
        case 't':
            num_threads = atoi(optarg);
            break;
        case 's':
            size = atoi(optarg);
            break;
        default:
            fprintf(stderr, "Использование: %s -t <потоки> -s <размер>\n",
                    argv[0]);
            return 1;
        }
    }

    if (num_threads < 1) {
        fprintf(stderr, "Потоков должно быть >= 1\n");
        return 1;
    }
    if (size < 1) {
        fprintf(stderr, "Размер должен быть >= 1\n");
        return 1;
    }

    printf("Потоков: %d, Размер массива: %d\n", num_threads, size);

    sem_init(&sem, 0, num_threads);

    int* arr = malloc(size * sizeof(int));
    if (!arr) {
        perror("malloc");
        return 1;
    }
    fill_random(arr, size);

    int depth = 30;
    double t0 = now_sec();
    quicksort_parallel(arr, 0, size - 1, depth);
    double t1 = now_sec();

    printf("Отсортировано: %s\n", is_sorted(arr, size) ? "да" : "НЕТ");
    printf("Создано потоков: %d\n", total_threads_created);
    printf("Время: %.4f сек\n", t1 - t0);

    sem_destroy(&sem);
    free(arr);
    return 0;
}