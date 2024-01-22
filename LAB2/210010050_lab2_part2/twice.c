#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char* argv[]){
    if(argc >= 2){
        int num = 2 * (atoi(argv[argc-1]));
        printf("Twice: Current process id: %d, Current result: %d\n", getpid(), num);
        sprintf(argv[argc-1], "%d", num);
        if(argc>2) execvp(argv[1], argv+1);
    }
    return 0;
}
