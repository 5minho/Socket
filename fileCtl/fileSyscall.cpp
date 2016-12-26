#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void error_handling(char* message) {
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}

int main() {
    ssize_t w_fd = 0;
    ssize_t r_fd = 0;
    char write_buf[] = "write text";
    char read_buf[100] = "";

    w_fd = open("data.in",O_WRONLY | O_CREAT | O_TRUNC);
    if(w_fd == -1)
        error_handling("open() error!");
    printf("file descriptor: %lu\n",w_fd);
    if(write(w_fd, write_buf, sizeof(write_buf)) == -1)
        error_handling("write() error!");
    close(w_fd);

    r_fd = open("data.in", O_RDONLY);
    if(r_fd == -1)
        error_handling("open error2!");
    printf("file descriptor : %lu\n",r_fd);
    if(read(r_fd, read_buf, sizeof(read_buf)) == -1)
        error_handling("read() error!");
    printf("%s\n", read_buf);
    close(r_fd);
    return 0;
}


