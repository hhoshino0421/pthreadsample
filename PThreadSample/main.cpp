#include <pthread.h>
#include <unistd.h>
#include <inttypes.h>
#include <stdio.h>
#include <mm_malloc.h>

#include <time.h>

#define THREADS     16

//構造体定義
struct fact_task {
    uint64_t    num;
    uint64_t    from;
    uint64_t    to;
    uint64_t    result;
};


//関数プロトタイプ定義
void* fact_worker(void* arg);
uint64_t factors_mp(uint64_t num,size_t threads_count);


int main(void) {

    clock_t start,end;
    start = clock();


    uint64_t    input = 20000000000;

    uint64_t    result = factors_mp(input,THREADS);

    printf("Factors of %" PRIu64 ":%" PRIu64 "\n", input, result);

    end = clock();
    printf("Process time : %.2f　seconds\n",(double)(end - start)/CLOCKS_PER_SEC);


    return 0;

}


void* fact_worker(void* arg) {

    struct fact_task * const task = (struct fact_task *)arg;
    task->result = 0;

    for (uint64_t i = task->from; i < task->to; i++) {

        if (task->num % i == 0) {
            task->result++;
        }

    }

    return NULL;

}


uint64_t factors_mp(uint64_t num,size_t threads_count) {

    struct fact_task*   tasks   = (struct fact_task*)malloc(threads_count * sizeof(*tasks));

    pthread_t*          threads = (pthread_t*)malloc(threads_count * sizeof(*threads));

    uint64_t  start = 1;
    size_t step = num / threads_count;


    for (size_t i = 0; i < threads_count; i++) {
        tasks[i].num    = num;
        tasks[i].from   = start;
        tasks[i].to     =  start + step;

        start           += step;
    }

    tasks[threads_count - 1].to = num + 1;

    for (size_t i = 0; i < threads_count; i++) {
        pthread_create(threads + 1, NULL, fact_worker, tasks + 1);
    }

    uint64_t  result = 0;

    for (size_t i = 0; i < threads_count; i++) {
        pthread_join(threads[i], NULL);
        result += tasks[i].result;
    }

    free(tasks);
    free(threads);

    return result;


}