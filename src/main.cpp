#include <iostream>
#include <string>
#include "repo.hpp"
#include "index.hpp"
#include "tree.hpp"
#include "commit.hpp"
#include "log.hpp"
#include "checkout.hpp"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cout << "Usage: vcc <command> [<args>]\n";
        return 1;
    }

    std::string command = argv[1];

    try
    {
        if (command == "init")
        {
            return RepoManager::init() ? 0 : 1;
        }

        if (!RepoManager::is_repo())
        {
            std::cerr << "[ERROR] Not a VCC repository (run 'vcc init' first)\n";
            return 1;
        }

        if (command == "add")
        {
            if (argc < 3)
            {
                std::cerr << "[ERROR] No file specified to add.\n";
                return 1;
            }
            IndexManager::add(argv[2]);
        }
        else if (command == "write-tree")
        {
            std::cout << TreeManager::write_tree() << "\n";
        }
        else if (command == "commit")
        {
            if (argc < 3)
            {
                std::cerr << "[ERROR] No commit message specified.\n";
                return 1;
            }
            std::cout << CommitManager::commit(argv[2]) << "\n";
        }
        else if (command == "checkout")
        {
            if (argc < 3)
            {
                std::cerr << "[ERROR] No commit hash specified.\n";
                return 1;
            }
            CheckoutManager::checkout(argv[2]);
        }
        else if (command == "log")
        {
            LogManager::show_log();
        }
        else
        {
            std::cerr << "[ERROR] Unknown command: " << command << "\n";
            return 1;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "[FATAL] " << e.what() << "\n";
        return 1;
    }

    return 0;
}