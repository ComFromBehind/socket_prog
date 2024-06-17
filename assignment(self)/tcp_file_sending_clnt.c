#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF_SIZE 1024

void error_handling(char*message);

char outputBuf[BUF_SIZE];
char inputBuf[BUF_SIZE];
char temp[BUF_SIZE];
char temp2[10];
char filename[BUF_SIZE];
int main(int argc, char * argv[]){
    int sock;

    struct sockaddr_in serv_adr;
    
    if(argc!=3){
        printf("Usage :  %s <IP> <port> \n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock==-1)
        error_handling("socket() error");

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_adr.sin_port = htons(atoi(argv[2]));

    if(connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)
        error_handling("connect() error");

    printf("원하시는 파일명을 입력하세요(확장자 포함) : ");
    scanf("%s", temp);
   
    

    
    int str_len = strlen(temp);
    char charlen = (char)str_len;
    outputBuf[0] = charlen;
    strcpy(&outputBuf[1], temp);
    write(sock, outputBuf, str_len+1);

    printf("서버가 처리중 ....\n");
    printf("파일이 없을 시 연결이 종료됩니다..\n");

    temp[0] = 'w'; // 원본과 구별하기 위해 임시로 만듬. 
    FILE*file = fopen(temp, "wb");

    if(file==NULL){
        error_handling("파일 생성 실패");
        exit(1);
    }

    int n;
    while((n=recv(sock, inputBuf, BUF_SIZE,0))>0){
        fwrite(inputBuf, sizeof(char), n, file);
        memset(inputBuf, 0, BUF_SIZE);
    }
    printf("파일 수신 완료!\n");
    
    fclose(file);

    outputBuf[0] = '\0';



    close(sock);
}

void error_handling(char*message){
    fputs(message,stderr);
    fputc('\n', stderr);
    exit(1);
}