#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char* argv[]){
    char s[] = "Hello World";
    int pid;
    for(int i=0; s[i]!='\0'; i++){
        printf("Character: %c, PID: %d\n", s[i], getpid());
        sleep(rand()%4 + 1);
        pid = fork();
        if(pid>0) break;
    }
    return 0; 
}