#ifndef LOG_HPP
#define LOG_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include "utils.hpp"

using namespace std;

namespace fs = std::filesystem;

class LogManager
{
public:
    static void show_log()
    {
        string current_hash = get_head_hash();

        if (current_hash.empty())
        {
            cout << "No commits yet.\n";
            return;
        }
        while (!current_hash.empty())
        {
            string content = read_object(current_hash);

            cout << "\033[33mcommit " << current_hash << "\033[0m\n";
            cout << parse_commit_details(content) << "\n";

            current_hash = find_parent_hash(content);
        }
    }

private:
    static string get_head_hash()
    {
        ifstream in(".vcc/refs/heads/main");
        string hash;
        if (in >> hash)
            return hash;
        return "";
    }

    static string read_object(const string &hash)
    {
        fs::path p = fs::path(".vcc") / "objects" / hash.substr(0, 2) / hash.substr(2);
        return read_file(p.string());
    }

    static string parse_commit_details(const std::string &content)
    {
        stringstream ss(content);
        string line, details;
        while (getline(ss, line))
        {
            if (line.empty())
                break;
            if (line.substr(0, 7) == "author")
                details += line + "\n";
        }
        string message;
        getline(ss, message);
        return details + "    " + message;
    }

    static string find_parent_hash(const string &content)
    {
        stringstream ss(content);
        string line;
        while (getline(ss, line))
        {
            if (line.substr(0, 7) == "parent ")
            {
                return line.substr(7);
            }
        }
        return "";
    }
};

#endif