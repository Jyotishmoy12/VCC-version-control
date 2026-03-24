#ifndef INDEX_HPP
#define INDEX_HPP

#include <map>
#include <string>
#include <filesystem>
#include "utils.hpp"
#include "sha1.hpp"
#include <set>

namespace fs = std::filesystem;

class IndexManager
{
public:
    static void add(const std::string &filename)
    {

        if (should_ignore(filename))
        {
            std::cout << "[SKIP] " << filename << " is in .vccignore\n";
            return;
        }
        if (!fs::exists(filename))
        {
            throw std::runtime_error("File not found: " + filename);
        }
        std::string content = read_file(filename);
        SHA1 checksum;
        checksum.update(content);
        std::string hash = checksum.final();
        fs::path obj_dir = fs::path(".vcc") / "objects" / hash.substr(0, 2);
        fs::create_directories(obj_dir);
        write_file(obj_dir / hash.substr(2), content);
        auto index = read_index();
        index[filename] = hash;
        write_index(index);
        std::cout << "[SUCCESS] Added " << filename << " to staging area.\n"; // Put it here!
    }

private:
    static bool should_ignore(const std::string &filename)
    {
        if (!fs::exists(".vccignore"))
            return false;
        std::ifstream in(".vccignore");
        std::string pattern;
        while (std::getline(in, pattern))
        {
            if (pattern.empty() || pattern[0] == '#')
                continue;
            if (filename == pattern)
                return true;
            if (pattern.size() > 1 && pattern[0] == '*' && pattern[1] == '.')
            {
                std::string ext = pattern.substr(1);
                if (filename.size() >= ext.size() &&
                    filename.compare(filename.size() - ext.size(), ext.size(), ext) == 0)
                {
                    return true;
                }
            }
        }
        return false;
    }

private:
    static std::map<std::string, std::string> read_index()
    {
        std::map<std::string, std::string> index;
        std::ifstream in(".vcc/index");
        std::string line;
        while (std::getline(in, line))
        {
            size_t space = line.find(' ');
            if (space != std::string::npos)
            {
                index[line.substr(space + 1)] = line.substr(0, space);
            }
        }
        return index;
    }

    static void write_index(const std::map<std::string, std::string> &index)
    {
        std::ofstream out(".vcc/index");
        for (auto const &[path, hash] : index)
        {
            out << hash << " " << path << "\n";
        }
    }
};

#endif