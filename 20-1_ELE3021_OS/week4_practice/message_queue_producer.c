#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct msgbuf{
    long msgtype;
    char mtext[20];
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
    strcpy(sndbuf.mtext,"hello there");

    if(msgsnd(key_id, (void*)&sndbuf, sizeof(struct msgbuf), IPC_NOWAIT)==-1){
        perror("msgsnd error: ");
    }
    printf("Send success\n");

    return 0;
}