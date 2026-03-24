#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <fstream>
#include <sstream>
#include <filesystem>

namespace fs = std::filesystem;

inline std::string read_file(const std::string &path)
{
    std::ifstream file(path, std::ios::binary);
    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

inline void write_file(const fs::path &path, const std::string &content)
{
    std::ofstream out(path, std::ios::binary);
    out << content;
}

#endif