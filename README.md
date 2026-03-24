# VCC (Version Control Clone)

## Overview

VCC is a lightweight, custom-built Version Control System clone developed in C++. It provides fundamental version control functionalities designed to track changes, manage repositories, and handle basic operations similar to Git.

## Getting Started (Compilation & Testing)

If you'd like to copy the code, build the project yourself, and test the version control functionalities:

1. **Clone the repository:**
   ```bash
   git clone https://github.com/Jyotishmoy12/VCC-version-control.git
   cd VCC-version-control
   ```

2. **Compile the executable:**
   Since VCC utilizes modern C++ features like `<filesystem>`, you must compile it with C++17 or higher. To compile using `g++`, run the following command from the root of the project:
   ```bash
   g++ -std=c++17 src/main.cpp -o vcc.exe
   ```

3. **Run your first command:**
   Once successfully compiled, you can initialize your first repository test:
   ```powershell
   .\vcc.exe init
   ```

### Features (What It Can Do)
- **Local Repository Initialization:** Creates a `.vcc` database to track history securely.
- **Content-Addressable Staging:** Stages files by hashing their exact binary content using SHA-1.
- **Ignore Rules:** Respects a custom `.vccignore` file to prevent staging unwanted tracking files.
- **Snapshot Commits:** Records linear history by tying a root Tree to an Author, Message, and Parent Commit.
- **History Logging:** Traverses and displays the full linear commit history.
- **Time-Travel Checkouts:** Restores the working directory to precisely match any previous commit snapshot.

### Current Limitations
- **Flat Tree Assumption:** Currently optimized for files residing in the root directory; recursive subdirectory hierarchy tracking is not yet fully dynamic.
- **Single Branching:** Hardcoded to track a single linear timeline on `main`. No branch creation or merging capability yet.
- **Local Only:** Does not support remote networking operations like `push`, `pull`, `clone`, or `fetch`.
- **Missing `status`/`diff`:** No command-line interface exists yet to quickly check for unstaged modifications or visually compare line-by-line file differences.
- **Hardcoded Metadata:** Commit author information is currently hardcoded within the `CommitManager` source code.

### Architecture

For a detailed breakdown of the internal components, system flow diagram, and a comprehensive summary of all functions, please see the [Architecture Documentation](ARCHITECTURE.md).

### Commands Guide

To learn how to use VCC, initialize repositories, stage files, and track history, check out the [Commands Reference](COMMANDS.md). Please ensure you run all commands using the `.\vcc.exe` format.

### Technical Details

Curious about how VCC utilizes SHA-1 hashing, content-addressable storage, or what resides inside the `.vcc` directory? Dive into the [Technical Details](TECHNICAL_DETAILS.md).

### Core Concepts

The project is structured around several core version control concepts:
- **Repository Management:** Initialization and tracking of the `.vcc` directory structure.
- **Object Hashing:** Implementation of SHA-1 hashing to securely identify file states and commits.
- **Index/Staging Area:** Tracking files added to the staging environment before a commit.
- **Commits & Trees:** Storing snapshots of the project directory and associating them with commit metadata.
- **History & Logs:** Maintaining a linear history of commits.
- **Checkout:** Restoring working directory files to a specific previous state.
