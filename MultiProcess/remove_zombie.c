#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

void remove_zombie(int sig) {
    int status = 0;
    pid_t id = 0;
    if(sig == SIGCHLD) {
        id = waitpid(-1, &status, WNOHANG);
        printf("Removed proc id : %d", id);
        if(WIFEXITED(status))
            printf("Child send %d \n", WEXITSTATUS(status));
    }
}

int main(int argc, char* argv[]) {
    pid_t pid = fork();
    struct sigaction act;
    int i = 0;
    act.sa_handler = remove_zombie;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;

    sigaction(SIGCHLD, &act, 0);
    
    if(pid == 0) {
        puts("HI I'm child process");
        return 24;
    } else {
        printf("Child proc id : %d\n", pid);
        pid = fork();
        
        if(pid == 0) {
            puts("HI I'm child process");
            return 12;
        } else {
            printf("Child proc id : %d\n", pid);
        }
        for(i = 0 ; i < 3; ++i) {
            puts("wait...");
            sleep(10);
        }
    }
    return 0;
}   
