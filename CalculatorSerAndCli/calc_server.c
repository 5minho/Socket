#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
void error_handling(char* message);
int calculation(int* result, int* num, char op);
void error_handling(char* message) {
    fputs(message, stderr);
    fputc('\n',stderr);
    exit(1);
}

int calculation(int* result, int* num, char op) {
    switch(op) {
        case '+':
            *result += *num; break;
        case '-':
            *result -= *num; break;
        case '*':
            *result *= *num; break;
        case '/':
            *result /= *num; break;
    }
    printf("result : %d, num : %d, op : %c  \n",*result, *num, op);
    return *result;
}


int main(int argc, char* argv[]) {
    int ser_sock;
    int cli_sock;
    
    int i = 0;
    int read_byte = 0;
    int result= 0;
    int oper_cnt = 0;
    int recv_cnt = 0;
    int recv_len = 0;

    struct sockaddr_in ser_addr;
    struct sockaddr_in cli_addr;
    char expression[BUF_SIZE];
    char op;

    socklen_t cli_adr_size;
    
    if(argc != 2) {
        printf("Usage : %s <port>\n",argv[0]);
        exit(1);
    }

    if((ser_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) != -1) {
    }
    else {
        error_handling("socket() error!!");
    }
    
    memset(&ser_addr, 0, sizeof(ser_addr));
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_port = htons(atoi(argv[1]));
    ser_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    if((bind(ser_sock, (struct sockaddr*) &ser_addr, sizeof(ser_addr))) == -1) {
        error_handling("bind() error!!");
    }

    if(listen(ser_sock, 5) == -1) {
        error_handling("listen() error!!");
    }
    
    cli_adr_size = sizeof(cli_addr);
    cli_sock = accept(ser_sock, (struct sockaddr*)&cli_addr, &cli_adr_size);
    if(cli_sock == -1) error_handling("accept() error!!");
    read(cli_sock, &oper_cnt, 1);

    while((4 * oper_cnt + 1) > recv_len) {
        recv_cnt = read(cli_sock, &expression[recv_len], BUF_SIZE);
        recv_len += recv_cnt;
    }

    result = expression[0];
    for(i = 1 ; i < oper_cnt ; ++i) {
        calculation(&result, (int*)&expression[i * 4], expression[4 * oper_cnt]);    
    }

    printf("%d\n", result);
    write(cli_sock, (char*)&result, sizeof(result));
    close(cli_sock);
    close(ser_sock);
    return 0;
}
