#ifndef COMMIT_HPP
#define COMMIT_HPP

#include <string>
#include <fstream>
#include <chrono>
#include <ctime>
#include "tree.hpp"
#include "utils.hpp"

class CommitManager {
public:
    static std::string commit(const std::string& message) {
        std::string tree_hash = TreeManager::write_tree();
        std::string parent_hash = get_head_hash();
        std::stringstream commit_content;
        commit_content << "tree " << tree_hash << "\n";
        if (!parent_hash.empty()) {
            commit_content << "parent " << parent_hash << "\n";
        }
        commit_content << "author Jyotishmoy Deka\n"
        commit_content << "\n" << message << "\n";

        std::string content = commit_content.str();
        SHA1 checksum;
        checksum.update(content);
        std::string commit_hash = checksum.final();

        std::string dir_name = commit_hash.substr(0, 2);
        std::string obj_name = commit_hash.substr(2);
        fs::path obj_dir = fs::path(".vcc") / "objects" / dir_name;
        
        fs::create_directories(obj_dir);
        write_file(obj_dir / obj_name, content);
        update_head(commit_hash);

        return commit_hash;
    }

private:
    static std::string get_head_hash() {
        std::ifstream in(".vcc/refs/heads/main");
        if (!in) return "";
        std::string hash;
        in >> hash;
        return hash;
    }

    static void update_head(const std::string& hash) {
        fs::create_directories(".vcc/refs/heads");
        write_file(".vcc/refs/heads/main", hash);
    }
};

#endif