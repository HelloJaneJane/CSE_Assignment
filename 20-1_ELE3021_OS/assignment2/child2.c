#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#define limit 15

// test.txt 파일을 r 모드로 열어 2 초 간격으로 첫번째 줄을 읽고 이를 화면에 출력
    
int main(){
    FILE* fp;

    while(1){
        sleep(2);
        fp = fopen("./test.txt", "r");
        fseek(fp,0,SEEK_SET);
        char buf[10];
        fgets(buf,sizeof(buf),fp);
        buf[strlen(buf)-1]='\0'; // \n없애기
        printf("Child2 red %s.\n",buf);

        if(atoi(buf)==limit-1) break;
    }

    fclose(fp);

    exit(0);
}