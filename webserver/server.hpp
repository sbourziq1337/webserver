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
#include <vector>

class Request
{
public:
    std::string mthod;
    std::string path;
    std::string version;
    std::string root;
    std::map<std::string, std::string> post_res;
};
void parsing_method(Request &rec, const std::string line);
void parsing_Get(std::map<std::string, std::string> haed, std::string path, int &fd);
void ft_error(const char *msg);
void response(std::string name_file, int fd, std::string header);
long getFileSize(const std::string &filename);
bool hasEnding(const std::string &fullString, const std::string &ending);
std::string getContentType(const std::string &filename);
bool is_file(const std::string &path);
bool is_directory(const std::string &path);
void parsing_Post(std::map<std::string, std::string> head, std::string body, std::string path, int &fd);
std::string urlDecode(const std::string &str);

