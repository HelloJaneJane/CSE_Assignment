## 운영체제 Assignment5

## Mutex: Peterson's algorithm

### 과제 A

c++의 atomic_int 변수를 적용하여 Peterson’s algorithm 구현

* COUNTING_NUMBER는 2000000이상
* Actual Count == Expected Count 되도록 Peterson‘s Algorithm 수정하여 구현
* atomic 라이브러리를 사용하기 위해 c 가 아닌 c++ 에서 진행
* critical_section_variable 변수를 제외한 나머지 공유변수들은 atomic_int 자료형으로 선언

#### 1. 자료구조 설명

```c++
atomic_int turn(0);
atomic_int flag0(0), flag1(0);
```

critical section에 접근 중인 스레드 번호를 나타내는 변수 turn과 접근을 원하는지 여부를 나타내는 변수 flag를 atomic_int로 선언하여 프로세스들이 공유 변수를 atomic하게 사용할 수 있게 한다.

#### 2. 동기화 방법 설명

Peterson 알고리즘으로 스레드 번호를 lock/unlock하는 동기화 방법을 사용한다.

```c++
lock(*thread_num);
critical_section_variable++;
unlock(*thread_num);
```

critical section에 접근하기 전에 lock 함수를, 접근이 끝난 후에 unlock 함수를 실행한다. 각 함수는 현재 스레드 번호를 인자로 받는다.

```c++
void lock(int self){
	(self==0?flag0:flag1)=1;
	turn=1-self;
	while((self==0?flag1:flag0)==1 && turn==1-self);
}
```

lock 함수는 해당 스레드의 flag를 1로 설정하여 cs로의 접근을 원함을 나타내고, turn을 상대 스레드로 바꿔준다.

```c++
void unlock(int self){
    (self==0?flag0:flag1)=0;
}
```

unlock 함수는 해당 스레드의 flag를 0으로 설정하여 cs로의 접근이 끝났음을 나타낸다.

상대 스레드의 flag가 1이고 turn이 상대 스레드라면 cs에 상대 스레드가 접근 중인 상태이므로 현재 스레드는 while loop에 갇힌다. 상대 스레드의 flag가 0이라면 현재 스레드가 cs에 접근할 수 있는 상태이므로 현재 스레드는 while loop를 빠져나와 cs에 접근하게 된다.

#### 3. 프로그램 구조 설명

```c++
pthread_t p0, p1;
int parameter[2] = {0,1};
pthread_create(&p0, NULL, func, (void*)&parameter[0]);
pthread_create(&p1, NULL, func, (void*)&parameter[1]);
```

func 함수를 실행하는 두 스레드를 생성하고 각각 0번, 1번의 번호를 부여한다.

```c++
void* func(void* s){
    int* thread_num = (int*)s;
    for(int i=0; i<COUNTING_NUMBER; i++){
        lock(*thread_num);
        critical_section_variable++;
        unlock(*thread_num);
    }
}
```

func 함수는 COUNTING_NUMBER번 cs_variable을 1씩 증가시킨다. cs_variable에 접근 전/후에 lock/unlock을 한다.

```c++
pthread_join(p0, NULL);
pthread_join(p1, NULL);
```

두 스레드의 실행이 모두 끝난 후 cs_variable을 출력해 결과를 확인한다.

#### 4. 컴파일 및 실행 결과

<img src="./A_assignment/resultA.png" alt="resultA" style="zoom:70%;" align="left"/>

두 스레드의 실행이 모두 끝난 후 cs_variable을 출력해 결과를 확인한다.

---

### 과제 B

shared memory 를 사용하여 프로세스 간의 Peterson’s algorithm 구현

* Peterson’s algorithm 측면 요구사항
  * thread 간이 아니라 process 간 Peterson’s algorithm 을 구현
  * 공유 변수 critical_section_variable 을 공유 메모리로 선언
* 프로그램 설계 측면 요구사항
  * 자식 프로세스 2 개를 생성하며 critical_section_variable 값을 조작하는 것은 자식 프로세스만 함
  * 각 자식 프로세스를 나타내는 id 를 선언하여 사용
  * 부모 프로세스는 모든 자식 프로세스가 종료될 때까지 기다림 (waitpid 사용)
  * 공유 메모리의 attach, detach, delete 과정이 코드 상에 포함되어 있어야 함
* Critical Section 측면 요구사항
  * critical_section_variable 의 값을 증가시키는 행위의 횟수로 사용하기 위해 COUNTING_NUMBER 를 선언하여 사용
  * critical_section_variable 의 값을 증가시키는 행위를 실제로 count 하기 위해 localcount 지역변수를 선언하여 사용
  * 실습과 동일하게 critical_section_variable 에 자식 프로세스 2 개가 동시에 접근하여 변수의 값을 1 씩 증가시킴

#### 1. 자료구조 설명

```c
typedef struct smStruct{
    int processidassign;
    int turn;
    int flag[2];
    int critical_section_variable;
}smStruct;
```

공유 변수들을 smStruct 구조체에 넣어 shared memory로 사용한다.

```c
// shared memory에 구조체 쓰기
smStruct* smstruct;
smstruct=shmaddr;
smstruct->turn=0;
smstruct->processidassign=0;
smstruct->flag[0]=0;
smstruct->flag[1]=0;
smstruct->critical_section_Variable=0;
```

shared memory address를 통해 포인터로 구조체 내부 값에 접근한다.

#### 2. 동기화 방법 설명

Peterson 알고리즘으로 프로세스 번호를 lock/unlock하는 동기화 방법을 사용한다.

```c++
lock(smstruct, myOrder);
smstruct->critical_section_variable++;
unlock(smstruct, myOrder);
```

critical section에 접근하기 전에 lock 함수를, 접근이 끝난 후에 unlock 함수를 실행한다. 자식 프로세스를 생성하면서 인자로 넘겨주는 문자열이 프로세스 번호를 나타내므로 이를 lock, unlock 함수의 인자로 사용해 해당 프로세스를 구별한다.

#### 함수의 작동 원리는 과제A 내용과 같다.

```c
void lock(smStruct* smstruct, char* myOrder){
    int self;
    if(myOrder[0]=='0') self=0;
    else if(myOrder[0]=='1') self=1;
    
    smstruct->flag[self]=1;
    smstruct->turn = 1-self;
    while(smstruct->flag[1-self]==1 && smstruct->turn==1-self);
}
```

```c++
void unlock(smStruct* smstruct, char* myOrder){
    int self;
    if(myOrder[0]=='0') self=0;
    else if(myOrder[0]=='1') self=1;

    smstruct->flag[self]=0;
}
```

#### 3. 프로그램 구조 설명

#### `parent.c`

1. shared memory를 shmget으로 생성, shmat으로 프로세스 메모리 공간에 attach한 후 shared memory에 smstruct를 write한다.

2. ```c
   pid_t pid = fork();
       if(pid==0){
           printf("Child Process is created: myOrder = 0, process pid = %d\n",getpid());
           execl("./child","./child","0",NULL);
       }
       else{
           pid = fork();
           if(pid==0){
           printf("Child Process is created: myOrder = 1, process pid = %d\n",getpid());
               execl("./child","./child","1",NULL);          
           }
   ```

   fork와 execl로 두 개의 자식 프로세스를 생성하고, 이때 각 프로세스에게 번호를 나타내는 “0”과 “1”이라는 문자열을 인자로 넘겨준다.

   #### 		`child.c`

    1. ```c
       int main(int argc, char* argv[]){
           char* myOrder = argv[1];
       ```

       프로세스 생성시 받은 문자열로 myOrder에 프로세스 번호를 저장한다.

   	2.  shared memory를 shmget으로 가져오고 shmat으로 프로세스 메모리 공간에 attach한 후 shared memory의 smstruct를 read한다.

   	3. ```c
       int localcount=0;
       for(int i=0; i<COUNTING_NUMBER; i++){
       localcount++;
       lock(smstruct, myOrder);
       smstruct->critical_section_variable++;
       unlock(smstruct, myOrder);
       }
       printf(" Child %s finish! local count = %d\n",myOrder,localcount);
       ```

       COUNTING_NUMBER번 localcount와 cs_variable을 1씩 증가시킨다.

       공유 변수에 접근 전/후에 lock/unlock을 한다. 카운팅이 끝난 후 localcount를 출력한다.

   	4.  shared memory를 shmdt로 detach한다.

       

3. waitpid로 자식 프로세스가 모두 종료될 때까지 기다린 후 cs_variable을 출력해 결과를 확인한다.

4. shared memory를 shmdt로 detach, shmctl로 삭제한다.

#### 4. 컴파일 및 실행 결과

<img src="./B_assignment/resultB.png" alt="resultB" style="zoom:70%;" align="left"/>

동기화 문제가 거의 해결되어 cs_variable 값이 COUNTING_NUMBER를 두 배한 값과 비슷해짐을 알 수 있다.