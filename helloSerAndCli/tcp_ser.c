#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void error_handling(char* message);

int main(int argc, char* argv[]) {
    int ser_sock;
    int cli_sock;

    struct sockaddr_in ser_addr;
    struct sockaddr_in cli_addr;
    socklen_t cli_addr_size;

    char message[]="Hello Socket!!";

    if(argc != 2) {
        printf("Usage : %s <port>\n",argv[0]);
        exit(1);
    }

    ser_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(ser_sock == -1)
        error_handling("socket() error");

    memset(&ser_addr, 0, sizeof(ser_addr));
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_addr.s_addr=htonl(INADDR_ANY);
    ser_addr.sin_port=htons(atoi(argv[1]));

    if(bind(ser_sock, (struct sockaddr*) &ser_addr, sizeof(ser_addr)) == -1)
        error_handling("bind() error");

    if(listen(ser_sock, 5) == -1)
        error_handling("listen() error");

    cli_addr_size = sizeof(cli_addr);
    cli_sock = accept(ser_sock, (struct sockaddr*)&cli_addr, &cli_addr_size);
    if(cli_sock == -1)
        error_handling("accept() error");

    write(cli_sock, message, sizeof(message));
    close(cli_sock);
    close(ser_sock);
    return 0;
}

void error_handling(char* message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
