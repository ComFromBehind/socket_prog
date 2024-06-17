#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF_SIZE 1024

char message[BUF_SIZE];
char receivemsg[BUF_SIZE];
char byteinfo[100];
char temp[100];
void error_handling(char*message);

int main(int argc, char *argv[]){
    int sock;
    struct sockaddr_in serv_addr;
    char message[20];  
    int str_len=0;

    int idx = 0, read_len = 0;

    if(argc!=3){
        printf("Usage : %s <IP> <port> \n", argv[0]);
        exit(1);
    }

    sock=socket(PF_INET, SOCK_STREAM, 0); // TCP 소켓 생성 라인이다. 
    if(sock ==-1)
        error_handling("socket() error");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1)
        error_handling("connect() error");
    
    read(sock, byteinfo, 4);
    int byte_len;
    
    byte_len = *(int*)&byteinfo;
    
    printf("byte_len : %d\n", byte_len);
    
    int rcnt = 0;
    int rlen = 0;
    while(byte_len>rlen){
        rcnt = read(sock, receivemsg, byte_len);
        if(rcnt==-1) 
            error_handling("read() error");
        rlen += rcnt;
    }
    printf("서버가 보낸 말 : %s\n", receivemsg);
    receivemsg[0] = '\0';

    printf("이번엔 클라이언트가 말할 차례, 문자열 입력 : ");
    
    fgets(temp, 100, stdin); //scanf로 찍으면 스페이스바 처리가 곤란해서 fgets로 바꿈. 

    int len = strlen(temp);

    *((int*)&message) = len;
    strcpy(&message[4], temp);

    write(sock, message, 4+len);

    byteinfo[0] = '\0';
    read(sock, byteinfo, 4);
    len = *(int*)&byteinfo;

    rcnt = 0;
    rlen = 0;
    
    message[0] = '\0';
    while(len>rlen){
        rcnt = read(sock, receivemsg, len);
        if(rcnt==-1)
            error_handling("read() error");
        rlen += rcnt;
    }
    printf("서버의 마지막 말 : %s", receivemsg);

    close(sock);
    return 0;
    
}

void error_handling(char*message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}