#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int global=0;

void* ThreadFunc(void* n){
    int* number = (int*)n;
    for(int i=0; i<2; i++){
        printf("%dth process: the global variable value is %d\n",*number,global++);
    }
    return 0;
}

int main(){
    pthread_t threadID[5];
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