#ifndef CHECKOUT_HPP
#define CHECKOUT_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include "utils.hpp"
#include "tree.hpp"

class CheckoutManager
{
public:
    static void checkout(const std::string &commit_hash)
    {
        std::string commit_content = read_object(commit_hash);
        std::string tree_hash = find_tree_hash(commit_content);
        if (tree_hash.empty())
            throw std::runtime_error("Invalid commit object");
        std::string tree_content = read_object(tree_hash);
        restore_tree(tree_content);
        write_file(".vcc/refs/heads/main", commit_hash);

        std::cout << "[SUCCESS] Checked out commit: " << commit_hash << "\n";
    }

private:
    static std::string read_object(const std::string &hash)
    {
        fs::path p = fs::path(".vcc") / "objects" / hash.substr(0, 2) / hash.substr(2);
        if (!fs::exists(p))
            throw std::runtime_error("Object not found: " + hash);
        return read_file(p.string());
    }

    static std::string find_tree_hash(const std::string &content)
    {
        std::stringstream ss(content);
        std::string line;
        if (std::getline(ss, line) && line.substr(0, 5) == "tree ")
        {
            return line.substr(5);
        }
        return "";
    }

    static void restore_tree(const std::string &tree_content)
    {
        std::stringstream ss(tree_content);
        std::string line;

        while (std::getline(ss, line))
        {
            if (line.empty())
                continue;

            std::stringstream line_ss(line);
            std::string mode, type, hash, name;
            if (line_ss >> mode >> type >> hash >> name)
            {
                if (type == "blob")
                {
                    std::string content = read_object(hash);
                    write_file(name, content);
                    std::cout << "  Restored: " << name << "\n";
                }
            }
        }
    }
};

#endif