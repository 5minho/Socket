#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define BUF_SIZE 30

int main(int argc, char* argv[]) {
    int fds[2];
    pid_t pid;
    char msg1[BUF_SIZE] = "";
    char msg2[BUF_SIZE] = "";
    pipe(fds);
    pid = fork();
    if(pid == 0) {
        strcpy(msg1, "I'm Child Process\n");
        write(fds[1], msg1, sizeof(msg1));
       // sleep(2);
        read(fds[0], msg2, BUF_SIZE);
        puts(msg2);
    } else {
        strcpy(msg2, "I'm Parent Process\n");
        read(fds[0], msg1, BUF_SIZE);
        puts(msg1);
        write(fds[1], msg2, sizeof(msg2));
        sleep(3);
    }
    return 0;
}
