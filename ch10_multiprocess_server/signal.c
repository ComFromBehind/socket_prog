#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void timeout(int sig)
{
    if(sig==SIGALRM)
        puts("Time out!");
    alarm(2);
}

void keycontrol(int sig)
{
    if(sig==SIGINT)
        puts("CTRL+C pressed");
}

int main(int argc, char*argv[])
{
    int i;
    signal(SIGALRM, timeout);
    signal(SIGINT, keycontrol);
    alarm(2);

    for(i=0;i<3;i++)
    {
        puts("wait...");
        sleep(10);
    }
    return;
}
//이 코드는 alarm 을 통해 SIGALRM 을 부르면서 다시 2초뒤 alarm이 오게끔 짜여져있다.
//이 때 잠들어 있던 프로세스는 alarm 신호를 받으면 block 상태에서 깨게 된다. 