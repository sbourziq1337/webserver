#include "server.hpp"
#include <sys/stat.h>
long getFileSize(const std::string &filename)
{
    struct stat s;
    if (stat(filename.c_str(), &s) == 0)
        return (s.st_size);
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
    std::ostringstream oss;
    oss << "Content-Length: " << getFileSize(name_file) << "\r\n";
    oss << "Connection: close" << "\r\n\r\n";
    header += oss.str();

    const char *new_head = header.c_str();
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
bool is_file(const std::string &path)
{
    struct stat s;
    if (stat(path.c_str(), &s) == 0)
        return S_ISREG(s.st_mode);
    return false;
}

bool is_directory(const std::string &path)
{
    struct stat s;
    if (stat(path.c_str(), &s) == 0)
        return S_ISDIR(s.st_mode);
    return false;
}
std::string urlDecode(const std::string &str)
{
    std::ostringstream decoded;
    for (size_t i = 0; i < str.length(); ++i)
    {
        if (str[i] == '%' && i + 2 < str.length() &&
            std::isxdigit(str[i + 1]) && std::isxdigit(str[i + 2]))
        {
            std::string hex = str.substr(i + 1, 2);
            int value;
            std::istringstream iss(hex);
            iss >> std::hex >> value;
            char ch = static_cast<char>(value);

            decoded << ch;
            i += 2;
        }
        else if (str[i] == '+')
            decoded << ' ';
        else
            decoded << str[i];
    }
    return decoded.str();
}
std::string remove_slash(std::string path) {
    size_t pos;
    while ((pos = path.find("//")) != std::string::npos) {
        path.replace(pos, 2, "/");
    }
    if (path.empty()) {
        return "/";
    }

    if (path[0] != '/') {
        return "/" + path;
    }

    return path;
}
void parsing_method(Request &rec, const std::string line) {
    std::istringstream input(line);
    std::string filename;
    input >> rec.mthod >> filename >> rec.version;
    rec.root = "/home/sbourziq/Desktop/web/webserver";
    filename = urlDecode(filename);
    filename = remove_slash(filename);
    rec.path = rec.root + (filename == "/" ? "/index.html" : filename);
 //   std::cout << filename << std::endl;
}

