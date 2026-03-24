#ifndef TREE_HPP
#define TREE_HPP

#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include "index.hpp"
#include "sha1.hpp"
#include "utils.hpp"

struct TreeEntry
{
    std::string mode;
    std::string type;
    std::string hash;
    std::string name;
};

class TreeManager
{
public:
    static std::string write_tree()
    {
        auto index = read_index();

        std::vector<TreeEntry> entries;
        for (auto const &[path, hash] : index)
        {
            entries.push_back({"100644", "blob", hash, path});
        }
        std::sort(entries.begin(), entries.end(), [](const TreeEntry &a, const TreeEntry &b)
                  { return a.name < b.name; });
        std::stringstream tree_content;
        for (const auto &entry : entries)
        {
            tree_content << entry.mode << " " << entry.type << " "
                         << entry.hash << " " << entry.name << "\n";
        }

        std::string content = tree_content.str();
        SHA1 checksum;
        checksum.update(content);
        std::string tree_hash = checksum.final();

        std::string dir_name = tree_hash.substr(0, 2);
        std::string obj_name = tree_hash.substr(2);
        fs::path obj_dir = fs::path(".vcc") / "objects" / dir_name;

        fs::create_directories(obj_dir);
        write_file(obj_dir / obj_name, content);

        return tree_hash;
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
};

#endif