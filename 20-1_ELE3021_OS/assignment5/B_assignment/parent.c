#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <wait.h>

#define COUNTING_NUMBER 2*1000*1000

typedef struct smStruct{
    int processidassign;
    int turn;
    int flag[2];
    int critical_section_variable;
}smStruct;

int main(){
    int shmid;
    void* shmaddr;
    int ret;

    // shared memory 생성
    shmid = shmget((key_t)1234, 1024, IPC_CREAT|0666);
    if(shmid<0){
        perror("get fail\n");
        return 0;
    }

    // shared memory를 프로세스 메모리 공간에 attach
    shmaddr = shmat(shmid, (void*)0, 0);
    if(shmaddr==(char*)-1){
        perror("attach fail\n");
        return 0;
    }

    // shared memory에 구조체 쓰기
    smStruct* smstruct;
    smstruct=shmaddr;
    smstruct->turn=0;
    smstruct->processidassign=0;
    smstruct->flag[0]=0;
    smstruct->flag[1]=0;
    smstruct->critical_section_variable=0;

    // 자식 프로세스 만들기
    int status;
    pid_t pid = fork();
    if(pid==0){
        printf("Child Process is created: myOrder = 0, process pid = %d\n",getpid());
        execl("./child","./child","0",NULL);
    }
    else{
        pid = fork();
        if(pid==0){
        printf("Child Process is created: myOrder = 1, process pid = %d\n",getpid());
            execl("./child","./child","1",NULL);          
        }
        // 부모
        else{
            printf("Parent Process is waiting for Child Processes.\n");
            waitpid(pid, &status, 0);
            printf("Actual Count: %d | Expected Count: %d\n",smstruct->critical_section_variable,COUNTING_NUMBER*2);
        }
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
        perror("delete fail\n");
        return 0;
    }

    return 0;
}