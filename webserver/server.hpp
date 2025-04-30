#pragma once
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <fstream>
#include <map>
#include <cstdlib>
#include <sstream>

class Request
{
public:
    std::string mthod;
    std::string path;
    std::string version;
};
void parsing_Get(std::map<std::string, std::string> haed, std::string path, int &fd);
void ft_error(const char *msg);
void response(std::string name_file, int fd, const char *header);