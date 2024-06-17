#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF_SIZE 1024

char outputBuf[BUF_SIZE];
char inputBUF[BUF_SIZE];
char temp[BUF_SIZE];

void error_handling(char*message);

int main(int argc, char*argv[]){
    
    int filename_len = 0;
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_adr, clnt_adr;
    socklen_t clnt_adr_sz;

    if(argc!=2){
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock ==-1)
        error_handling("socket() error");
    
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family= AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)
        error_handling("bind() error");
    
    if(listen(serv_sock, 10)==-1)
        error_handling("listen() error");
    
    clnt_adr_sz = sizeof(clnt_adr);
    clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
    if(clnt_sock==-1)
        error_handling("accept() error");
    
    read(clnt_sock, &filename_len, 1);
    
    printf("client가 보낸 파일의 길이 : %d\n", filename_len);

    int rcnt = 0;
    int rlen = 0;
    while(rlen < filename_len){
        rcnt = read(clnt_sock, inputBUF, BUF_SIZE-1);
        if(rcnt==-1)
            error_handling("read() error");
        
        rlen += rcnt;
    }

    printf("클라이언트가 요구하는 파일명 : %s\n", inputBUF);
    
    printf("해당 파일이 서버에 있는지 탐색 중 ....\n");
    if(access(inputBUF, F_OK)==0){
        printf("파일 존재\n");

        FILE* file = fopen(inputBUF, "rb");

        if(file==NULL){
            error_handling("file open fail\n");
            exit(1);
        }

        while(fread(outputBuf, sizeof(char), BUF_SIZE, file)>0){
            if(send(clnt_sock, outputBuf, BUF_SIZE, 0)==-1){
                error_handling("파일 전송 실패");
                exit(1);
            }
            memset(outputBuf, 0, BUF_SIZE);
        }

        fclose(file);
        printf("파일 송신 완료\n");
    }
    else{
        printf("파일 없음\n");

        printf("파일이 존재하지 않으므로 즉시 종료\n");
        close(clnt_sock);
        close(serv_sock);

        return 0;

    }
    inputBUF[0] = '\0';

    close(clnt_sock);
    close(serv_sock);

    return 0;
    
}

void error_handling(char*message){
    fputs(message,stderr);
    fputc('\n', stderr);
    exit(1);
}