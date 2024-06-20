#define _XOPEN_SOURCE 200
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

char *message;

void mykeyinput(int sig){
    if(sig==SIGINT){
        printf("정말로 종료하시겠습니까? (Y/N)\n");

        message = malloc(50*sizeof(char));
        scanf("%49s", message);
        printf("message : %s\n", message);
        if(!strcmp(message, "y")||!strcmp(message, "Y"))
            exit(1);
        
    }
}

int main(int argc, char *argv[])
{
    struct sigaction act;
    act.sa_handler = mykeyinput;
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);
    sigaction(SIGINT, &act, 0);

    while(1){
        sleep(1);
        printf("if you want exit, press 'ctrl + c'\n");

    }

    free(message);

    return 0;
}