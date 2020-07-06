#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>

#define PIPENAME "./named_pipe_file"
#define MSG_SIZE 80
#define DATA_SIZE 10

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
    
    printf("Hello, this is B process. I'll give the data to A.\n");
    printf("You can input the data until %d times.\n",DATA_SIZE);

    // 사용자(쉘) 입력받고 write하기
    int cnt=0;
    while(cnt++<DATA_SIZE){
        sleep(1);
        scanf("%s",msg);
        ret = write(fd, msg, sizeof(msg));
        if(ret<0){
            printf("write fail\n");
            return 0;
        }
    }
    return 0;
}