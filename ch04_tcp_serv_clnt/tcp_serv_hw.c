#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
void error_handling(char*message);

int main(int argc, char * argv[]){
    int serv_sock;
    int clnt_sock;

    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size;
    int write_len = 0;
    int idx =0 ;
    char message[] = "Hello World!";
    int write_cnt = 0;

    if(argc!=2){
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }

    serv_sock=socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1)
        error_handling("socket() error");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); 
    serv_addr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1)
        error_handling("bind() error");

    if(listen(serv_sock, 5)==-1)
        error_handling("listen() error");
    
    clnt_addr_size = sizeof(clnt_addr);
    clnt_sock= accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
    if(clnt_sock ==-1)
        error_handling("accept() error");
    

    // write(clnt_sock, message, 4);
    // write(clnt_sock, message+4, 4);
    // write(clnt_sock, message+8, 4);
    // write(clnt_sock, message+12, sizeof(message)-12);

    while(write_len=write(clnt_sock, &message[idx++],1)){
       
        printf("write cnt : %d, write_len : %d\n", write_cnt, write_len);
        write_cnt += write_len;
    }

    //write를 이렇게 짜게 되면 , read와는 다른 방식으로 실행된다. read는 입력버퍼를 하나씩 읽고서 더이상 읽을 문자가 없으면
    //종료되었지만, write는 굉장히 큰 수 (1000~2000) 까지 읽다가 -1을 내고서 종료하게 된다. 해서 차라리 while문으로 1바이트씩이 아니라
    //그 위의 코드처럼 정확한 사이즈를 주는 방식으로하면 문제가 없지만, 일단은 내가 짠 방식에는 문제가 있다. 

    // for(int i=0;i<50000;i++){
    //     printf("writer sleep %d \n", i);
    // }

    printf("we write %d\n", write_cnt);
    close(clnt_sock);
    

    return 0;

}

void error_handling(char*message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
