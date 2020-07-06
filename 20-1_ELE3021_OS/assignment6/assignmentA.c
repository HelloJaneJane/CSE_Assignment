#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>

#define COUNTING_NUMBER 100

int cur_writer=0; // CS에 접근한 writer id
int cur_count=0; // writer가 CS에 접근한 총 횟수

int readcnt=0; // 현재 CS에 접근 중인 reader 수
sem_t S, wrt;

void writer(void* arg){
    int id = *(int*)arg;
    for(int i=0; i<COUNTING_NUMBER; i++){
        usleep(100000);

        // Lock
        sem_wait(&wrt);

        // Critical Section
        printf("\tWriter%d (%d)\n",id,i);
        cur_writer=id;
        cur_count++;

        // Unlock
        sem_post(&wrt);
    }
}

void reader(void* arg){
    int id = *(int*)arg;
    for(int i=0; i<COUNTING_NUMBER; i++){
        usleep(30000);

        // Lock
        sem_wait(&S);
        readcnt++;
        if(readcnt==1) sem_wait(&wrt);
        sem_post(&S);

        // Critical Section
        printf("Reader%d (%d): cur_writer=%d, cur_count=%d\n",id,i,cur_writer,cur_count);

        // Unlock
        sem_wait(&S);
        readcnt--;
        if(readcnt==0) sem_post(&wrt);
        sem_post(&S);
    }
}

int main(){
    sem_init(&S,0,1);
    sem_init(&wrt,0,1);

    pthread_t threadReader[2];
    pthread_t threadWriter[5];

    int readerID[2]={1,2};
    int writerID[5]={1,2,3,4,5};

    for(int i=0; i<2; i++){
        pthread_create(&threadReader[i],NULL,(void*)&reader,(void*)&readerID[i]);
    }
    for(int i=0; i<5; i++){
        pthread_create(&threadWriter[i],NULL,(void*)&writer,(void*)&writerID[i]);
    }
    
    for(int i=0; i<2; i++){
        pthread_join(threadReader[i],NULL);
    }
    for(int i=0; i<5; i++){
        pthread_join(threadWriter[i],NULL);
    }

    sem_destroy(&S);
    sem_destroy(&wrt);
}