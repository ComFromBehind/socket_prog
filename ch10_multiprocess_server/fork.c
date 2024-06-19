#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>


int gval = 10;
int main(int argc, char*argv[])
{
    pid_t pid;
    int lval = 20;
    gval++; lval+=5;

    pid=fork();
    
    if(pid==0)
        gval+=2, lval+=2;
    
    else   
        gval-=2, lval-=2;
    
    if(pid==0)
        printf("Child proc : [%d, %d]\n", gval, lval);      //여기서 pid값은 자식 프로세스가 return값으로 돌려준 0이 된다.
    else
        printf("Parent proc : [%d, %d, %d]\n", gval, lval, pid);    //여기서 pid 값은 자식 프로세스의 pid값을 의미한다.
    
    return 0;
}

//이 코드에서 변수 pid값이 현재 실행중인 프로세스의 pid값이 아니다!! (의미)