#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30
void error_handling(char *message);

char * temp = "sex";

int main(int argc, char *argv[])
{
    char temp[BUF_SIZE];
    int send_sock;
    struct sockaddr_in broad_adr;
    FILE *fp;
    char buf[BUF_SIZE];
    int so_brd = 1;
    if(argc!=3)
    {
        printf("Usage : %s <Broadcast IP> <port> \n", argv[0]);
        exit(1);
    }

    send_sock = socket(PF_INET, SOCK_DGRAM, 0);
    memset(&broad_adr, 0, sizeof(broad_adr));
    broad_adr.sin_family = AF_INET;
    broad_adr.sin_addr.s_addr = inet_addr(argv[1]);
    broad_adr.sin_port = htons(atoi(argv[2]));

    setsockopt(send_sock, SOL_SOCKET, SO_BROADCAST, (void*)&so_brd, sizeof(so_brd));
    if((fp = fopen("news.txt", "r"))==NULL)
        error_handling("fopen() error");
    
    while(1)
    {
        if(fgets(buf, BUF_SIZE, fp)==NULL)
        {
            if(feof(fp)){
                
                
                break;
            }
        }
        
        sendto(send_sock, buf, strlen(buf), 0, (struct sockaddr*)&broad_adr, sizeof(broad_adr));
        sleep(2);
    }
    printf("여기서 한번 끝남 .\n");
    printf("buf : %s\n" , buf);
    
        sleep(5);
        printf("종료");
        close(send_sock);
    
    
    return 0;
}

void error_handling(char * message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

