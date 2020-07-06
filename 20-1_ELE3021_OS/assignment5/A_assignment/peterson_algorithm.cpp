#include <atomic>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
using namespace std;

#define COUNTING_NUMBER 2*1000*1000

int critical_section_variable=0;

atomic_int turn(0);
atomic_int flag0(0), flag1(0);

void lock(int self){
    (self==0?flag0:flag1)=1;
    turn=1-self;
    while((self==0?flag1:flag0)==1 && turn==1-self);
}

void unlock(int self){
    (self==0?flag0:flag1)=0;
}

void* func(void* s){
    int* thread_num = (int*)s;
    for(int i=0; i<COUNTING_NUMBER; i++){
        lock(*thread_num);
        critical_section_variable++;
        unlock(*thread_num);
    }
    return 0;
}

int main(){
    pthread_t p0, p1;
    int parameter[2] = {0,1};

    pthread_create(&p0, NULL, func, (void*)&parameter[0]);
    pthread_create(&p1, NULL, func, (void*)&parameter[1]);
    
    pthread_join(p0, NULL);
    pthread_join(p1, NULL);

    printf("Actual Count: %d | Expected Count : %d\n", critical_section_variable, COUNTING_NUMBER*2);

    return 0;
}