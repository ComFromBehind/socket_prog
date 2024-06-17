#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>

int main(void){
    int fd1, fd2, fd3;

    fd1 = socket(PF_INET, SOCK_STREAM,0);
    fd2 = open("test.dat", O_CREAT|O_WRONLY|O_TRUNC);
    fd3 = socket(PF_INET, SOCK_DGRAM,0);

    printf("file descriptor 1 : %d\n", fd1);
    printf("file descriptor 2 : %d\n", fd2);
    printf("file descriptor 3 : %d\n", fd3);

    close(fd1);
    close(fd2);     //test.dat파일에 권한을 부여해주지 않으면 이따금 -1을 띄울 수 있다. (읽기 에러)
    close(fd3);     //이럴때는 chmod u+rw test.dat을 쳐주자. 그러고 다시 컴파일하면 된다.

}

//파일 디스크립터는 3부터 할당이 되기 시작하는데 이는
//0은 표준 입력, 1은 표준 출력, 2는 표준에러로 먼저 할당되어있기 때문이다. 