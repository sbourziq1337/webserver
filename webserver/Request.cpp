#include "server.hpp"

long getFileSize(const std::string &filename)
{
    std::ifstream file(filename.c_str(), std::ios::binary);
    if (file)
    {
        file.seekg(0, std::ios::end);
        std::streampos size = file.tellg();
        if (size != std::streampos(-1))
            return static_cast<long>(size);
    }
    return -1;
}
void response(std::string name_file, int fd, std::string header)
{
    std::ifstream file(name_file.c_str(), std::ios::binary);
    if (!file.is_open())
    {
        const char *error404 = "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\n\r\n"
                               "<html><body><h1>404 Not Found</h1></body></html>";
        send(fd, error404, strlen(error404), 0);
        close(fd);
        return;
    }
    header += "content-length: " + getFileSize(name_file) + "\r\n\r\n";
    const char* new_head = header.c_str();
    char buffer[4096];
    send(fd, new_head, strlen(new_head), 0);

    while (file.read(buffer, sizeof(buffer)) || file.gcount() > 0)
    {
        send(fd, buffer, file.gcount(), 0);
    }

    close(fd);
}
bool hasEnding(const std::string &fullString, const std::string &ending)
{
    if (fullString.length() >= ending.length())
    {
        return fullString.compare(fullString.length() - ending.length(), ending.length(), ending) == 0;
    }
    return false;
}
std::string getContentType(const std::string &filename)
{
    if (hasEnding(filename, ".html"))
        return "text/html";
    if (hasEnding(filename, ".css"))
        return "text/css";
    if (hasEnding(filename, ".js"))
        return "application/javascript";
    if (hasEnding(filename, ".png"))
        return "image/png";
    if (hasEnding(filename, ".jpg") || hasEnding(filename, ".jpeg"))
        return "image/jpeg";
    return "application/octet-stream";
}

void parsing_Get(std::map<std::string, std::string> head, std::string path, int &fd)
{
    if (path.empty())
        path = "index.html";
    std::ifstream file(path.c_str());
    if (!file.is_open())
    {
        std::string header = "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\n";
        response("not_found.html", fd, header);
    }
    else
    {
        std::string header = "HTTP/1.1 200 OK\r\nContent-Type: " + getContentType(path) + "\r\n\r\n";
        response(path, fd, header);
    }
    std::cout << "File size is: " << getFileSize(path) << " bytes" << std::endl;
}
