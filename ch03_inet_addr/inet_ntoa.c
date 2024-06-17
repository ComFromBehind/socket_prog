#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

int main(int argc, char*argv[]){
    struct sockaddr_in addr1, addr2;
    char * str_ptr;
    char str_arr[20];

    addr1.sin_addr.s_addr = htonl(0x1020304);
    addr2.sin_addr.s_addr = htonl(0x1010101);

    str_ptr = inet_ntoa(addr1.sin_addr);
    strcpy(str_arr, str_ptr);       //저장해주지 않으면 다음에 inet_ntoa가 호출되었을 때 이전 정보를 잃어버린다.

    printf("Dotted-Decimal notation1 : %s \n", str_ptr);
    
    inet_ntoa(addr2.sin_addr);
    printf("Dotted-Decimal notation2 : %s \n", str_ptr); //임시 정보
    printf("Dotted-Decimal notation3 : %s \n", str_arr); //저장해놓은 정보
    return 0;
}