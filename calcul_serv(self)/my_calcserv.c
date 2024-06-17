#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
void error_handling(char*message);
int num[100];

int opcnt = 0;
char op;
int recv_cnt = 0;
int recv_len = 0;
int main(int argc, char*argv[])
{
    int serv_sock, clnt_sock;

    char message[BUF_SIZE];
    int str_len, i;
    
    struct sockaddr_in serv_adr, clnt_adr;
    socklen_t clnt_adr_sz;

    if(argc!=2) {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }

    serv_sock= socket(PF_INET, SOCK_STREAM, 0); //TCP 소켓 생성
    if(serv_sock==-1)
        error_handling("socket() error");

    memset(&serv_adr, 0, sizeof(serv_adr)); //serv_adr 멤버 변수 0으로 초기화
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)
        error_handling("bind() error");

    if(listen(serv_sock, 5)==-1)
        error_handling("listen() error");
    
    clnt_adr_sz = sizeof(clnt_adr);

    
    clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
    if(clnt_sock==-1)
        error_handling("accept() error");
    else
        printf("Connected client %d \n", i+1);
        
    recv_cnt=0;             //해당 변수들을 계속해서 사용하는 이유는 입력을 다 받아서 특정 변수 공간에 저장하기 전까지 다음 작업으로 넘어가는 것을
    recv_len = 0;           //막기 위해서다. 상대가 보내는 값의 사이즈를 알 수 없어서 2개의 장치를 썼다. 
    printf("Input operand count!! : \n");
    while(1){
        recv_cnt = read(clnt_sock, &message[recv_len], BUF_SIZE-1);    // 1. while 문으로 계속해서 read를 해줄 것.
        if(recv_cnt==-1)                                               // 2. 입력의 마지막으로 '\n'가 들어오는 지 확인해줄 것.
            error_handling("read() error!");
        recv_len += recv_cnt;                                   //이때에도 서버입장에서는 '\n'를 기준으로 처리할 수 있으나, 클라이언트가 서버에게 받았을 때는 다르다.
        if(message[recv_len-1]=='\n') break;
    }
    opcnt = atoi(message);
    printf("opcnt : %d\n", opcnt);
    
    for(int i=0;i<opcnt+1;i++){
        recv_cnt = 0;
        recv_len = 0;
        
        while(1){   
            recv_cnt = read(clnt_sock,&message[recv_len], BUF_SIZE-1);
            if(recv_cnt==-1)
                error_handling("read() error!");
            recv_len += recv_cnt;
            if(message[recv_len-1]=='\n') break;
        }
        write(clnt_sock,message, recv_len);

        if(i!=opcnt)
        {
            num[i] = atoi(message);
            printf("%d \n", num[i]);
        }
        else{
            op = message[0];
            printf("%c \n", op );
        }
    }
    
    puts("calculating...\n");
    
    int ans = 0;
    if(op=='+'){
        
        for(int i=0;i<opcnt;i++){
            ans+=num[i];
        }
    }
    else if(op=='-'){
        for(int i=1;i<opcnt;i++){
            ans-=num[i];
        }
        ans+=num[0];
    }
    else if(op=='*'){
        ans = num[0];
        for(int i=1;i<opcnt;i++){
            ans*=num[i];
        }
    }
    else{
        printf("calculating error!\n");
    }

    printf("sending client answer..\n");

    sprintf(message, "%d", ans);
    write(clnt_sock, message, BUF_SIZE);


    printf("server role finish..\n");

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