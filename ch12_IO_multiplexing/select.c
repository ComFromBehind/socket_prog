#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/select.h>
#define BUF_SIZE 30

int main(int argc, char * argv[])
{
    fd_set reads, temps;
    int result, str_len;
    char buf[BUF_SIZE];
    struct timeval timeout;

    FD_ZERO(&reads);
    FD_SET(0, &reads);

    /*
    timeout.tv_sec = 5;     //이 위치에서 값을 설정하면 안된다. 
    timeout.tv_usec = 5000; //timeout변수에 저장된 값이 timeout까지 남은 값이기 때문이다. 
    */

    while(1)
    {
        temps = reads;  //원본을 남겨놓기 위해서 하는 과정이다. 일반적으로 select함수를 쓰면 자주 쓰는 방법이다.

        timeout.tv_sec = 5;
        timeout.tv_usec = 0;
        result = select(1, &temps, 0,0, &timeout);
        if(result==-1)
        {
            puts("select() error!");
            break;
        }

        else if(result==0)
        {
            puts("Time-out!");
        }
        else
        {
            if(FD_ISSET(0, &temps))
            {
                str_len = read(0, buf, BUF_SIZE);
                buf[str_len] = 0;
                printf("message from console : %s",buf);
            }
        }
        
    }
}