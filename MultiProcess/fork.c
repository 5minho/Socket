#include <stdio.h>
#include <unistd.h>

int main() {
    pid_t pid = 0;
    
    int rval = 10,
        lval = 20;

    rval += 2;
    lval += 4;
    pid = fork();
    
    if(pid == 0) {
        rval += 3;
        lval -= 3;
    } 
    rval -= 3;
    lval += 3;

    if(pid == 0) {
        printf("Child Process - rval : %d, lval : %d\n",rval, lval);
    }
    else {
        printf("Parent Process - rval : %d, lval : %d\n",rval, lval);
    }
    return 0;
}
