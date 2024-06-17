#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF_SIZE 1024

void error_handling(char*message);

char message[BUF_SIZE];
char receivemsg[BUF_SIZE];
char temp[100];
char byteinfo[100];
int main(int argc, char * argv[]){
    int serv_sock;
    int clnt_sock;

    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size;

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
    
    int st_num = 6;
    char*s = "hello?";
    
    *((int*)&message[0]) = st_num;
    strcpy(&message[4], s);

    write(clnt_sock, message, 4+st_num);
    message[0] = '\0';
   

    printf("여기까지는 성공\n");

    read(clnt_sock, byteinfo, 4);
    st_num = *(int*)&byteinfo;


    int rcnt = 0;
    int rlen = 0;
    while(st_num > rlen)
    {
        rcnt = read(clnt_sock, receivemsg, st_num);
        if(rcnt==-1){
            error_handling("read() error!");
        }
        rlen += rcnt;
    }
    
    printf("client가 보낸 말 : %s\n", receivemsg);
    receivemsg[0]='\0';

    printf("server가 마지막으로 보내는 말, 문자열 입력 : ");
    
    fgets(temp, 100, stdin);
    
    int len = strlen(temp);

    *((int*)&message) = len;
    strcpy(&message[4], temp);

    write(clnt_sock, message, len+4);
    
    close(clnt_sock);
    close(serv_sock);

    return 0;

}

void error_handling(char*message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
