#include <stdio.h>
#include <unistd.h>
#include <wait.h>

int main() {

    FILE* fp;
    int status;
       
   // 자식 프로세스 1 생성
   pid_t pid = fork();
   if(pid==0){
       printf("Child Process 1 (pid: %d) is created.\n",getpid());
       execl("./child1",NULL);
   }
   else{
       // 자식 프로세스 2 생성
       pid = fork();
       if(pid==0){
           printf("Child Process 2 (pid: %d) is created.\n",getpid());
           execl("./child2",NULL);          
       }
       // 부모
       else{
           printf("Parent Process (waiting for Child Processes)\n");
           waitpid(pid, &status, 0);
           printf("Child processes are exit.\n");
       }
   }

}