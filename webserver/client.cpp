#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFFER_SIZE 255

void ft_error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    int sockfd, portnb, n;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE];

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <IP address> <port>\n", argv[0]);
        exit(1);
    }

    // 1. Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        ft_error("Error opening socket");

    // 2. Prepare server address
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    portnb = atoi(argv[1]);
    serv_addr.sin_port = htons(portnb);
    in_addr_t addr = inet_addr("127.0.0.1");
    if (addr == INADDR_NONE)
        ft_error("Invalid address");
    
    serv_addr.sin_addr.s_addr = addr;
    
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        ft_error("Connection failed");

    // 4. Chat loop
    while (1)
    {
        bzero(buffer, BUFFER_SIZE);
        printf("You: ");
        fgets(buffer, BUFFER_SIZE, stdin);

        n = write(sockfd, buffer, strlen(buffer));
        if (n < 0)
            ft_error("Error writing to socket");

        bzero(buffer, BUFFER_SIZE);
        n = read(sockfd, buffer, BUFFER_SIZE);
        if (n < 0)
            ft_error("Error reading from socket");

        printf("Server: %s", buffer);
        if (strncmp("Bye", buffer, 3) == 0)
            break;
    }

    close(sockfd);
    return 0;
}
