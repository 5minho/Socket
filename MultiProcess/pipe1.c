#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define BUF_SIZE 30

int main(int argc, char* argv[]) {
    char msg[BUF_SIZE] = "";
    int fd[2];
    pipe(fd); 
    pid_t pid = fork();
    if(pid == 0) {
        strcpy(msg,"I'm Child Process");
        write(fd[1], msg, sizeof(msg));
    } else {
        read(fd[0], msg, BUF_SIZE);
        puts(msg);
    }
    return 0;
}
