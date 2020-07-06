#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>

#define N 6

sem_t chopstick[N];

// chp[i] -- phil[i] -- chp[(i+1)%6]

void dining(void* arg){
    int id = *(int*)arg;

    // odd (L-type)
    if(id%2){
        printf("Philosopher%d: waiting for L\n",id);
        sem_wait(&chopstick[id]);
        printf("Philosopher%d: get chopstick%d(L)\n",id,id);
        printf("Philosopher%d: waiting for R\n",id);
        sem_wait(&chopstick[(id+1)%N]);
        printf("Philosopher%d: get chopstick%d(R)\n",id,(id+1)%N);

        printf("Philosopher%d: eating\n",id);

        sem_post(&chopstick[id]);
        sem_post(&chopstick[(id+1)%N]);
    }

    // even (R-type)
    else{
        printf("Philosopher%d: waiting for R\n",id);
        sem_wait(&chopstick[(id+1)%N]);
        printf("Philosopher%d: get chopstick%d(R)\n",id,(id+1)%N);
        printf("Philosopher%d: waiting for L\n",id);
        sem_wait(&chopstick[id]);
        printf("Philosopher%d: get chopstick%d(L)\n",id,id);

        printf("Philosopher%d: eating\n",id);

        sem_post(&chopstick[(id+1)%N]);
        sem_post(&chopstick[id]);
    }
}

int main(){
    for(int i=0;i<N;i++){
        sem_init(&chopstick[i],0,1);
    }

    pthread_t philosopher[N];
    int id[N];
    for(int i=0;i<N;i++) id[i]=i;

    for(int i=0; i<N; i++){
        pthread_create(&philosopher[i],NULL,(void*)&dining,(void*)&id[i]);
    }
    
    for(int i=0; i<N; i++){
        pthread_join(philosopher[i],NULL);
    }

    for(int i=0; i<N; i++){
        sem_destroy(&chopstick[i]);    
    }
}