#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define PIPENAME "./named_pipe_file"
#define DATA_SIZE 10

int main(){
    int ret;
    char msg[512];
    int fd;
    pid_t pid;

    // PIPENAME인 named pipe가 존재하는지 조사 
    ret = access(PIPENAME, F_OK);
    if(ret==0){
        unlink(PIPENAME); // 있으므로 지운다
    }

    // named pipe 생성
    ret = mkfifo(PIPENAME, 0666);
    if(ret<0){
        printf("create fail\n");
        return 0;
    }

    // named pipe 열기 (읽기쓰기 가능하게)
    fd = open(PIPENAME, O_RDWR);
    if(fd<0){
        printf("open fail\n");
        return 0;
    }

    printf("Hello, this is A process. Give me the data.\n");
    printf("I can receive the data until %d times.\n",DATA_SIZE);

    // read하기
    int cnt=0;
    while(cnt++<DATA_SIZE){
        ret = read(fd, msg, sizeof(msg));
        if(ret<0){
            printf("read fail\n");
            return 0;
        }
        printf("%s\n",msg);
    }
    return 0;
}