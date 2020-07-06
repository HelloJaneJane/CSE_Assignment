#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define ARGUMENT_NUMBER 20
#define ll long long

ll result=0;
ll results[ARGUMENT_NUMBER];

void* ThreadFunc(void* n){
    ll number = *((ll*)n);
    printf("number = %lld\n", number);
    ll ret=0;
    for(ll i=0; i<25000000; i++){
        ret += number;
    }
    // 현재 스레드 결과 값 저장
    results[number]=ret;
    return 0;
}

int main(){
    pthread_t threadID[ARGUMENT_NUMBER];
    ll argument[ARGUMENT_NUMBER];

    for(ll i=0; i<ARGUMENT_NUMBER; i++){
        argument[i]=i;
    }
    
    // 스레드 생성
    for(ll i=0; i<ARGUMENT_NUMBER; i++){
        pthread_create(&(threadID[i]), NULL, ThreadFunc, (void*)&argument[i]);
    }
    // 생성 완료되었을 때 문구 출력
    printf("Main Thread is waiting for Child Thread!\n");

    // 스레드 종료되면 각 스레드 결과 값 합치기
    for(ll i=0; i<ARGUMENT_NUMBER; i++){
        pthread_join(threadID[i], NULL);
        result+=results[i];
    }
    
    printf("result = %lld\n", result);
    return 0;
}