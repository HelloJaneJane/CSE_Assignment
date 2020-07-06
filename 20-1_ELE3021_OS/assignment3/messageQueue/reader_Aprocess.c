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

int main(int argc, char** argv){
    key_t key_id;
    struct msgbuf rsvbuf;
    int msgtype=3;

    key_id = msgget((key_t)1234, IPC_CREAT|0666);
    if(key_id==-1){
        perror("msgget error: ");
        return 0;
    }

    int cnt=0;
    while(cnt<DATASIZE){
        if(msgrcv(key_id, (void*)&rsvbuf, sizeof(struct msgbuf), msgtype, 0)==-1){
            perror("msgrcv error: ");
        }
        else{
            printf("The %dth received message is: %s\n", cnt++, rsvbuf.mtext);
        }
    }

    return 0;
}