#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "math.h"


int main(int argc, char *argv[]) {

    int sock;
    struct sockaddr_in servaddr, cliaddr;
    char client_message[2000], result[2000];

    memset(client_message, '\0', sizeof(client_message));

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
        perror("socket");
        return 1;
    }

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_addr = { .s_addr = INADDR_ANY },   
        .sin_port = htons(2020)
    };

    int rc = bind(sock, (struct sockaddr *) &addr, sizeof(addr));
    if (rc == -1) {
        printf("Eror with connection");
    } 

    socklen_t len = sizeof(cliaddr);

    while (1) {
        ssize_t cnt;
        
        cnt = recvfrom(sock, client_message, 2000, 
                    MSG_WAITALL, ( struct sockaddr *) &cliaddr,
                    &len);
        if (cnt < 0) {
            perror("recvfrom");
            exit(EXIT_FAILURE);
        }

        printf("Client: %s\n", client_message);

        calculate(client_message, result);
        
        printf("Result: %s\n", result);
        
        cnt = sendto(sock, result, strlen(result), 
            MSG_CONFIRM, (const struct sockaddr *) &cliaddr,
                len);
        if (cnt < 0) {
            perror("sendto");
            exit(EXIT_FAILURE);
        }
        
        memset(client_message, '\0', sizeof(client_message));
        memset(result, '\0', sizeof(result));
    }

    rc = close(sock);
    if (rc == -1) {
        perror("close");
        return 1;
    } 

    return 0;
}