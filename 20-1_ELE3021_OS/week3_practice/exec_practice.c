#include <stdio.h>
#include <unistd.h>

int main(void){
    execl("./exec_sample",NULL);
    printf("This sentence must not be printed!!\n");
    return 0;
}