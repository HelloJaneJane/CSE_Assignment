#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MSGSIZE 50
#define DATASIZE 10

struct msgbuf{
    long msgtype;
    char mtext[MSGSIZE];
};

int main(void){
    key_t key_id;
    struct msgbuf sndbuf;

    key_id = msgget((key_t)1234, IPC_CREAT|0666);
    if(key_id==-1){
        perror("msgget error: ");
        return 0;
    }

    sndbuf.msgtype=3;
    
    int cnt=0;
    while(cnt<DATASIZE){
        char msgbuf[MSGSIZE];
        scanf("%s",msgbuf);
        strcpy(sndbuf.mtext,msgbuf);

        if(msgsnd(key_id, (void*)&sndbuf, sizeof(struct msgbuf), IPC_NOWAIT)==-1){
            perror("msgsnd error: ");
        }
        printf("Sending %dth message is succeed\n",cnt++);
    }

    return 0;
}