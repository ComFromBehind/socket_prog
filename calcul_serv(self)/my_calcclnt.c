#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
void error_handling(char*message);

int main(int argc, char *argv[])
{
    int sock;
    char message[BUF_SIZE];
    int str_len, recv_len, recv_cnt;
    struct sockaddr_in serv_adr;

    if(argc!=3) {
        printf("Usage : %s <IP> <PORT> \n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock==-1)
        error_handling("socket() error!");
    
    memset(&serv_adr, 0, sizeof(serv_adr) );
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_adr.sin_port = htons(atoi(argv[2]));

    if(connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)
        error_handling("connect() error!");

    else
        puts("Connected........");

        printf("opcount input : ");
        fgets(message, BUF_SIZE, stdin);
        write(sock, message, strlen(message));

        int opcount = atoi(message);
        printf("now time to input %d number.\n",opcount);
        
        for(int i=0;i<opcount+1;i++){
            if(i<opcount)
                printf("Input %d number : ", i+1);
            else
                printf("Input Operator : ");
            fgets(message,BUF_SIZE,stdin);

            str_len = write(sock, message, strlen(message));

            recv_len = 0;

            while(recv_len < str_len)
            {
                recv_cnt = read(sock, &message[recv_len], BUF_SIZE);
                if(recv_cnt ==-1)
                    error_handling("read() error!");
                recv_len += recv_cnt;
            }

            message[recv_len] = 0;
            printf("server gets number(or operator) : %s\n", message); // 서버가 처리하고 돌려준 값
        }

        printf("now waiting server's reply... \n");

        recv_len = 0;
        recv_cnt = 0;
        while(1){
    
            recv_cnt = read(sock, &message[recv_len], BUF_SIZE);
          
            if(recv_cnt ==-1)
                error_handling("final read() error!");
            recv_len+=recv_cnt;
            
            if(message[recv_len-1]==0) break;   //클라이언트가 서버에게 받았을 때는 null 문자를 받았을 때, 종료가 되도록 하였다. 
                                                //클라이언트가 입력할 때는 '\n'입력이 있어서 이를 가지고 서버에서는 처리하였다. 
        }
        fflush(stdout);
        printf("the answer is %s\n", message); //이따금씩 계산 결과(message)가 먼저 출력되는 경우가 있음. 그래서 flush해줌.

        

    
    close(sock);
    return 0;
}

void error_handling(char * message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}