#include <stdio.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
    char *addr1 = "1.2.3.4";
    char *addr2 = "1.2.3.256";  //일부로 잘못된 예시를 집어넣음. 최대 255까지만 가능하다.

    unsigned long conv_addr = inet_addr(addr1);
    if(conv_addr==INADDR_NONE) //inet_addr 실패 시
        printf("Error occured! \n");
    else
        printf("Network ordered integer addr : %#lx \n", conv_addr);

    conv_addr = inet_addr(addr2);
    if(conv_addr==INADDR_NONE)
        printf("Error occured\n");
    else
        printf("Network ordered integer addr : %#lx \n\n", conv_addr);
    return 0;    
}