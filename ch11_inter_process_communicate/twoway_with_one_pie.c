#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

#define BUF_SIZE 30

int main(int argc, char * argv[])
{
    int fds[2];
    char str1[] = "Who are you?";
    char str2[] = "Thank you for your message";
    
    char buf[BUF_SIZE];
    pid_t pid;

    pipe(fds);
    pid = fork();
    if(pid==0)
    {
        write(fds[1], str1, sizeof(str1));
        sleep(2);
        //sleep(2); 부분을 주석처리하면 원하는 결과가 안나올 수 있다. 파이프에 먼저 온 프로세스가 데이터를 읽어가기 때문이다. 
        read(fds[0], buf, BUF_SIZE);
        printf("Child proc output : %s \n", buf);
    }
    else{
        read(fds[0], buf, BUF_SIZE);
        printf("Parent proc output: %s\n", buf);
        write(fds[1], str2, sizeof(str2));
        sleep(3);
    }
    return 0;
}