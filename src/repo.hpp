#ifndef REPO_HPP
#define REPO_HPP

#include <iostream>
#include <filesystem>
#include <string>
#include <fstream>

namespace fs = std::filesystem;

class RepoManager
{
public:
    static bool init()
    {
        fs::path vcc_dir = ".vcc";

        if (fs::exists(vcc_dir))
        {
            std::cerr << "[ERROR] .vcc already exists here." << std::endl;
            return false;
        }

        try
        {
            fs::create_directories(vcc_dir / "objects");
            fs::create_directories(vcc_dir / "refs" / "heads");
            std::ofstream index_file(vcc_dir / "index");

            std::cout << "[SUCCESS] Initialized at: " << fs::absolute(vcc_dir) << std::endl;
            return true;
        }
        catch (const fs::filesystem_error &e)
        {
            std::cerr << "[CRITICAL] Filesystem error: " << e.what() << std::endl;
            return false;
        }
    }
    static bool is_repo()
    {
        return fs::exists(".vcc") && fs::is_directory(".vcc");
    }
};

#endif