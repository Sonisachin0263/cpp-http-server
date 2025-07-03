#include "utils.hpp"
#include <fstream>
#include <sstream>

std::string read_file(const std::string& path) {
    std::ifstream file(path);
    if (!file) return "<h1>File Not Found</h1>";
    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}