#include "server.hpp"

void response_post(std::string name_file, int fd, std::string header)
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
void parsing_Post(std::map<std::string, std::string> head, std::string body, std::string path, int &fd)
{
    static std::map<std::string, std::string> post_res;

    std::map<std::string, std::string> form_data;
    if (head.count("Content-Type") && head["Content-Type"] == " application/x-www-form-urlencoded\r")
    {
        std::istringstream ss(body);
        std::string pair;
        while (std::getline(ss, pair, '&'))
        {
            size_t eq = pair.find('=');
            if (eq != std::string::npos)
            {
                std::string key =  pair.substr(0, eq);
                std::string value = pair.substr(eq + 1);
                form_data[key] = value;
            }
        }
    }

    std::string username = form_data["username"];
    std::string password = form_data["password"];
    std::cout << path << std::endl;
    std::cout << "username is " << username << ", password is " << password << std::endl;
    if (path == "/login.html" || path == "/login/index.html")
    {
        if (post_res.count(username) && post_res[username] == password)
            path = "./login/upload.html";
        else
            path = "./login/login_failed.html";
    }
    else if (path == "/login/singup.html" || path == "/singup/index.html")
    {
        post_res[username] = password;
        path = "./login/upload.html";
    }
    else if (is_directory(path))
    {
        path += "/index.html";
    }

    // Determine response
    if (is_file(path))
    {
        std::string header = "HTTP/1.1 200 OK\r\nContent-Type: " + getContentType(path) + "\r\n";
        response_post(path, fd, header);
    }
    else
    {
        std::string header = "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\n";
        response_post("./not_found.html", fd, header);
    }
}