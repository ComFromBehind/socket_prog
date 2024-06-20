#define _XOPEN_SOURCE 200
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

int status;

void reapingChild(int sig)
{
    if(sig==SIGCHLD){
        puts("child process is zombie and parent reap...");
        waitpid(-1, &status, WNOHANG );

        if(WIFEXITED(status)){
            printf("자식이 보낸 값 :  %d \n", WEXITSTATUS(status));
        }
    }
}

int main(int argc, char*argv[])
{
    int i;
    struct sigaction act;
    act.sa_handler = reapingChild;
    act.sa_flags= 0;
    sigemptyset(&act.sa_mask);
    sigaction(SIGCHLD, &act,0);

    pid_t pid = fork();

    if(pid==0){
        printf("i am child. i want to finish.\n");
        return 14;
    }
    else{
        printf("i am parent. \n");
        sleep(2);
    }
    return 0;
}