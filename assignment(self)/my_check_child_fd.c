#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>

void error_handling(char*message);

int main(int argc, char*argv[])
{   
    int sock1; int fd_for_check;
    
    sock1 = socket(PF_INET, SOCK_STREAM, 0);

    printf("i'm parent . my filedescriptor is %d\n", sock1);

    pid_t pid = fork();

    if(pid==0){
        fd_for_check = sock1;
        exit(fd_for_check);
    }

    else{
        sleep(2);
        int status;
        waitpid(-1, &status, WNOHANG);

        if(WIFEXITED(status)){
            if(WEXITSTATUS(status)==sock1){
                printf("파일 디스크립터가 동일하다!.\n");
            }
            else{
                printf("파일 디스크립터가 같지 않다!! 부모 : %d, 자식 : %d\n", sock1, WEXITSTATUS(status));
            }
        }
    }
    return 0;
}