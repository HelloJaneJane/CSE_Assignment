#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t mutex_lock;
int result=0;

void* ThreadFunc(void* n){
    int* number = (int*)n;
    for(int i=0; i<3; i++){
        pthread_mutex_lock(&mutex_lock);
        printf("%dth process: %d\n", *number, result++);
        pthread_mutex_unlock(&mutex_lock);
    }
    return 0;
}

int main(){
    pthread_t threadID[5];
    pthread_mutex_init(&mutex_lock, NULL);
    int argument[5] = {1,2,3,4,5};

    for(int i=0; i<5; i++){
        pthread_create(&(threadID[i]), NULL, ThreadFunc, (void*)&argument[i]);
    }

    printf("Main Thread is waiting for Child Thread!\n");

    for(int i=0; i<5; i++){
        pthread_join(threadID[i], NULL);
    }

    return 0;
}