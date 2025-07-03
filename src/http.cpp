#include "http.hpp"
#include "utils.hpp"

std::string build_http_response(const std::string& request) {
    if (request.find("GET / ") != std::string::npos) {
        std::string body = read_file("www/index.html");
        return "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: " + 
               std::to_string(body.size()) + "\r\n\r\n" + body;
    }

    std::string not_found = "<h1>404 Not Found</h1>";
    return "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\nContent-Length: " +
           std::to_string(not_found.size()) + "\r\n\r\n" + not_found;
}