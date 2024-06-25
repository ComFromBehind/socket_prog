#include <stdio.h>
#include <time.h>
#define BUF_SIZE 3

int main(int argc, char * argv[])
{
    FILE * fp1;
    FILE * fp2;
    char buf[BUF_SIZE];

    time_t start_time, end_time;

    

    fp1 = fopen("news.txt", "r");
    fp2 = fopen("cpy.txt", "w");

    start_time = time(NULL);

    while(fgets(buf, BUF_SIZE, fp1)!= NULL)
        fputs(buf, fp2);
    
    end_time = time(NULL);

    printf("실행시간 : %.2f\n", difftime(end_time, start_time));

    fclose(fp1);
    fclose(fp2);
    return 0;
}