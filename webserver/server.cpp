#include "server.hpp"
#include <iostream>
#include <fstream>
#include <map>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstdlib>
#include <sstream>

#define PORT 8080

void ft_error(const char *msg)
{
    perror(msg);
    exit(1);
}

void parsing_method(Request &rec, const std::string &line)
{
    int j = 0, count = 0;
    int i = 0;
    for ( i = 0; i < line.size(); i++)
    {
        if (line[i] == ' ')
        {
            if (count == 0)
                rec.mthod = line.substr(j, i - j);
            else if (count == 1)
                rec.path = line.substr((j + 1), i - j - 1);
            j = i + 1;
            count++;
        }
    }
    if (j < line.size())
        rec.version = line.substr(j, i - j - 1);
}


int main()
{
    sockaddr_in serv_add;
    serv_add.sin_family = AF_INET;
    serv_add.sin_addr.s_addr = INADDR_ANY;
    serv_add.sin_port = htons(PORT);

    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0)
        ft_error("Socket creation failed");

    int opt = 1;
    if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
        ft_error("setsockopt failed");

    if (bind(socket_fd, (struct sockaddr *)&serv_add, sizeof(serv_add)) < 0)
        ft_error("Binding failed");

    if (listen(socket_fd, 5) < 0)
        ft_error("Listening failed");

    std::cout << "Server is running on port " << PORT << "...\n";

    while (true)
    {
        sockaddr_in cli_add;
        socklen_t cli_len = sizeof(cli_add);
        int new_socket = accept(socket_fd, (struct sockaddr *)&cli_add, &cli_len);
        if (new_socket < 0) {
            perror("Accept failed");
            continue;  // Don't exit, continue to accept next connection
        }

        char buffer[4096];
        memset(buffer, 0, sizeof(buffer));
        ssize_t bytes_read = read(new_socket, buffer, sizeof(buffer) - 1);
        if (bytes_read < 0) {
            perror("Failed to read from socket");
            close(new_socket);
            continue;
        }

        // Save request to file
        std::ofstream MyFile("filename.txt");
        if (!MyFile.is_open()) {
            perror("Failed to open file for writing");
            close(new_socket);
            continue;
        }
        MyFile.write(buffer, bytes_read);
        MyFile.close();

        // Read and parse request
        std::ifstream ReadFile("filename.txt");
        if (!ReadFile.is_open()) {
            perror("Failed to open file for reading");
            close(new_socket);
            continue;
        }

        std::string method_line;
        if (!getline(ReadFile, method_line)) {
            perror("Failed to read method line");
            close(new_socket);
            continue;
        }

        Request obj;
        parsing_method(obj, method_line);

        std::map<std::string, std::string> head;
        std::string line;
        while (getline(ReadFile, line)) {
            if (line.empty())
                break;
            size_t x = line.find(":");
            if (x != std::string::npos) {
                std::string key = line.substr(0, x);
                std::string value = line.substr(x + 1);
                head[key] = value;
            }
        }

        if (obj.mthod == "GET" && obj.version == "HTTP/1.1")
            parsing_Get(head, obj.path, new_socket);

        ReadFile.close();
        close(new_socket);  // Close connection after handling request
    }

    close(socket_fd);
    return 0;
}
