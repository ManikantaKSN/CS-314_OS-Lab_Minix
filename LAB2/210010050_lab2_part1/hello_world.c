#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char* argv[]){
    char s[] = "Hello World";
    int i=0, pid;
    while(s[i]!='\0'){
        pid = fork();
        if(pid == 0){
            printf("%c (PID:%d)\n", s[i], getpid());
            sleep((rand()%4)+1);
            i++;
        }
        else{
            wait(NULL);
            break;
        }
    }
    return 0;
}
