#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main(void){
    int shmid;
    void* shmaddr;
    int ret;

    // shared memory 생성
    shmid = shmget((key_t)1234, 1024, IPC_CREAT|0666);
    if(shmid<0){
        perror("shmget\n");
        return 0;
    }

    // shared memory를 프로세스 메모리 공간에 attach
    shmaddr = shmat(shmid, (void*)0, 0);
    if(shmaddr==(char*)-1){
        perror("attach fail\n");
        return 0;
    }

    // shared memory 변수에 문자열 쓰기
    strcpy((char*)shmaddr, "Linux Programming");

    // shared memory를 detach
    ret = shmdt(shmaddr);
    if(ret==-1){
        perror("detach fail\n");
        return 0;
    }

    return 0;
}