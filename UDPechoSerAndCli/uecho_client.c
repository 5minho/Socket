#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30
void error_handling(char* message);

int main(int argc, char* argv[]) {
    int sock;
    char message[BUF_SIZE];
    int str_len;

    struct sockaddr_in ser_addr, 
                       from_addr;
    
    socklen_t adr_sz;

    if(argc != 3) {
        printf("Usage : %s <ip> <port>\n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(sock == -1) error_handling("socket() error!!");

    memset(&ser_addr, 0, sizeof(ser_addr));
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_port = htons(atoi(argv[2]));
    ser_addr.sin_addr.s_addr = inet_addr(argv[1]);

    
    while(1) {
        fputs("Insert message(q to quit) : ",stdout);
        fgets(message, sizeof(message), stdin);
        if(!strcmp(message,"q\n") || !strcmp(message, "Q\n")) break;
        str_len = sendto(sock, message, strlen(message), 0, (struct sockaddr*)&ser_addr, sizeof(ser_addr));
        adr_sz = sizeof(from_addr);
        recvfrom(sock, message, BUF_SIZE, 0, (struct sockaddr*)&from_addr, &adr_sz);
        message[str_len] = 0;
        printf("Message from server : %s",message);
    }
    close(sock);
    return 0;
}
void error_handling(char* message) {
    fputs(message, stderr);
    fputc('\n',stderr);
    exit(1);
}
