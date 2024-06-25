#include <stdio.h>
#include <fcntl.h>
#include <time.h>

#define BUF_SIZE 3

int main(int argc, char*argv[])
{
    time_t start_time, end_time;
    
    

    int fd1, fd2;
    int len;
    char buf[BUF_SIZE];

    fd1 = open("news.txt", O_RDONLY);
    fd2 = open("cpy.txt", O_WRONLY|O_CREAT|O_TRUNC);

    start_time = time(NULL);
    ctime(&start_time);
    while((len=read(fd1, buf, sizeof(buf)))>0)
        write(fd2, buf, len);
    
    end_time = time(NULL);
    ctime(&end_time);

    printf("실행 시간 : %.2f", difftime(end_time, start_time));
    close(fd1);
    close(fd2);
    return 0;
}