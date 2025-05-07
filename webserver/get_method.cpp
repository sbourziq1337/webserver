#include "server.hpp"

void parsing_Get(std::map<std::string, std::string> head, std::string path, int &fd)
{
    if (is_file(path))
    {
        std::string header = "HTTP/1.1 200 OK\r\nContent-Type: " + getContentType(path) + "\r\n";
        response(path, fd, header);
    }
    else if (is_directory(path))
    {
        path += "/index.html";
        std::string header = "HTTP/1.1 200 OK\r\nContent-Type: " + getContentType(path) + "\r\n";
        response(path, fd, header);
    }
    else
    {
        std::string header = "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\n";
        response("/home/akera/Desktop/webserver/not_found.html", fd, header);
    }
}