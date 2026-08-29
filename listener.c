#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 8080

int listening() {
    int sock;
    int client;
    if((sock = socket(AF_INET, SOCK_STREAM, 0))<0) {
        perror("[-] Connect to the interner");
        exit(1);
    }
    struct sockaddr_in server, clientaddr;
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = INADDR_ANY;
    int srvlen = sizeof(server);
    int clilen = sizeof(clientaddr);
    int opt = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    if(bind(sock, (struct sockaddr *)&server, sizeof(server))<0) {
        perror("[-] Failed to bind");
        exit(1);
    }
    if(listen(sock, 10)<0) {
        perror("[-] Server not listening");
    } else {
        printf("[+] Listening on port 8080\n");
    }
    client = accept(sock, (struct sockaddr *)&clientaddr, (socklen_t*)&clilen);
    char clientip[INET_ADDRSTRLEN];
    int cliport;
    inet_ntop(AF_INET, &clientaddr.sin_addr, clientip, sizeof(clientip));
    cliport = ntohs(clientaddr.sin_port);
    printf("[+] Connection from %s on port %d\n",clientip,cliport);
    while(1) {
        char in[1024];
        memset(in, 0, sizeof(in));
        char response[1024];
        memset(response, 0, sizeof(response));
        read(STDIN_FILENO, in, sizeof(in));
        while(in != NULL) {
            send(client, in, strlen(in), 0);
            recv(client, response, sizeof(response), 0);
            write(STDOUT_FILENO, response, strlen(response));
            break;
        };
    }
    return 0;
}

int main() {
    char *banner = "cat banner.txt";
    system(banner);
    listening();
    return 0;
}