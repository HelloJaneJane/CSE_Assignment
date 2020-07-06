#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define COUNTING_NUMBER 2*1000*1000

int critical_section_variable=0;

pthread_mutex_t mutex_lock;

void* func(void* s){
    int* thread_num = (int*)s;
    int localcount=0;

    for(int i=0; i<COUNTING_NUMBER; i++){
        pthread_mutex_lock(&mutex_lock);
        critical_section_variable++;
        pthread_mutex_unlock(&mutex_lock);
    }
    return 0;
}

int main(){
    pthread_t p1, p2;
    pthread_mutex_init(&mutex_lock, NULL);
    int parameter[2] = {0,1};

    pthread_create(&p1, NULL, func, (void*)&parameter[0]);
    pthread_create(&p2, NULL, func, (void*)&parameter[1]);
    
    pthread_join(p1, NULL);
    pthread_join(p2, NULL);

    printf("Actual Count: %d | Expected Count : %d\n", critical_section_variable, COUNTING_NUMBER*2);

    return 0;
}