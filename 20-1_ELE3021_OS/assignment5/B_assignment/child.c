#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define COUNTING_NUMBER 2*1000*1000

typedef struct smStruct{
    int processidassign;
    int turn;
    int flag[2];
    int critical_section_variable;
}smStruct;

void lock(smStruct* smstruct, char* myOrder){
    int self;
    if(myOrder[0]=='0') self=0;
    else if(myOrder[0]=='1') self=1;
    
    smstruct->flag[self]=1;
    smstruct->turn = 1-self;
    while(smstruct->flag[1-self]==1 && smstruct->turn==1-self);
}

void unlock(smStruct* smstruct, char* myOrder){
    int self;
    if(myOrder[0]=='0') self=0;
    else if(myOrder[0]=='1') self=1;

    smstruct->flag[self]=0;
}

int main(int argc, char* argv[]){
    int shmid;
    void* shmaddr;
    int ret;

    char* myOrder = argv[1];

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

    // shared memory에서 구조체 읽기
    struct smStruct* smstruct;
    smstruct=shmaddr;

    // 카운팅
    int localcount=0;
    for(int i=0; i<COUNTING_NUMBER; i++){
        localcount++;
        lock(smstruct, myOrder);
        smstruct->critical_section_variable++;
        unlock(smstruct, myOrder);
    }
    printf(" Child %s finish! local count = %d\n",myOrder,localcount);

    // shared memory를 detach
    ret = shmdt(shmaddr);
    if(ret==-1){
        perror("detach fail\n");
        return 0;
    }
    
    return 0;
}