#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#define limit 15

// test.txt 파일을 w 모드로 열어 1 초 간격으로 숫자와 개행문자를 쓰며 숫자는 1 씩 증가

int main(){
    FILE* fp = fopen("./test.txt", "w");

    for(int i=0;i<limit;i++){
        fseek(fp,0,SEEK_SET);
        fprintf(fp,"%d\n",i);
        printf("Child1 wrote %d.\n",i);
        sleep(1);
    }

    fclose(fp);

    exit(0);
}