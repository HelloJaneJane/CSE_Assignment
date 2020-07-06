#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>

#define PIPENAME "./named_pipe_file"
#define MSG_SIZE 80

int main(){
    int ret;
    char msg[MSG_SIZE];
    int fd;

    // named pipe 열기 (쓰기만 가능하게)
    fd = open(PIPENAME, O_WRONLY);
    if(fd<0){
        printf("open fail\n");
        return 0;
    }

    for(int i=0; i<3; i++){
        sleep(1);
        snprintf(msg,sizeof(msg),"Message[%i]",i);
        ret = write(fd, msg, sizeof(msg));
        if(ret<0){
            printf("write fail\n");
            return 0;
        }
    }
    return 0;
}