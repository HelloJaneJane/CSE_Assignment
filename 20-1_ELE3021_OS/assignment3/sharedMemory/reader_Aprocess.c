#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define DATASIZE 10
#define MSGSIZE 50

int main(void){
    int shmid;
    void* shmaddr;
    int ret;

    // shared memory 가져오기
    shmid = shmget((key_t)1234, 1024, IPC_CREAT|0666);
    if(shmid==-1){
        perror("access fail\n");
        return 0;
    }

    // shared memory를 프로세스 메모리 공간에 attach
    shmaddr = shmat(shmid, (void*)0, 0);
    if(shmaddr==(char*)-1){
        perror("attach fail\n");
        return 0;
    }

    int cnt=0;
    while(cnt<DATASIZE){
        printf("data read from shared memory: %s\n", (char*)shmaddr+MSGSIZE*(cnt++));
    }

    // shared memory를 detach
    ret = shmdt(shmaddr);
    if(ret==-1){
        perror("detach fail\n");
        return 0;
    }

    // shared memory 삭제
    ret = shmctl(shmid, IPC_RMID, 0);
    if(ret==-1){
        perror("remove fail\n");
        return 0;
    }

    return 0;
}