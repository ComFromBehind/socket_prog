#define _XOPEN_SOURCE 200
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

void read_childproc(int sig)
{
    int status;
    pid_t id = waitpid(-1, &status, WNOHANG);
    
    if(WIFEXITED(status))
    {
        printf("Removed proc id : %d \n", id);
        printf("Child send : %d \n", WEXITSTATUS(status));
    }
}

int main(int argc, char*argv[])
{
    pid_t pid;
    struct sigaction act;
    act.sa_handler = read_childproc;
    act.sa_flags=0;
    sigemptyset(&act.sa_mask);
    sigaction(SIGCHLD, &act, 0);

    pid= fork();
    if(pid==0)
    {
        puts("Hi! I'm child process");
        sleep(3);
        puts("now i die");
        return 12;
    }
    else{
        printf("Child proc id : %d \n", pid);
        pid = fork();
        if(pid==0)
        {
            puts("hi! I'm child process");
            sleep(10);
            puts("i am die");
            exit(24);
        }
        else{
            int i;
            printf("Child proc id : %d \n",pid);
            for(i=0;i<5;i++)
            {
                puts("wait...");
                sleep(5);
            }
        }
    }
    return 0;
}
//주의할 점은 signal을 처리하는 동안 또 다른 signal이 들어오면 무시될 수 있다.
//이 부분에 대해서는 공부해봐야 할듯.