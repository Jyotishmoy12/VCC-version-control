# VCC Command Reference

This document provides a comprehensive guide to all available commands in the VCC (Version Control Clone) system.

**Important Note:** When running these commands in a PowerShell or Windows command prompt, you must use the `.\vcc.exe` executable format to ensure the system executes the compiled binary in your current directory.

---

### `init`
Initialize a new, empty VCC repository.

**Usage:**
```powershell
.\vcc.exe init
```
**Details:** 
Creates a hidden `.vcc` directory in your current working directory. This directory structure contains everything VCC needs to track objects and refs. If a repository already exists in the folder, it will output an error.

---

### `add`
Add file contents to the staging area (index).

**Usage:**
```powershell
.\vcc.exe add <filename>
```
**Details:** 
Reads the specified file, generates a SHA-1 hash of its content, stores the content as a Blob in `.vcc/objects`, and records the mapping in the `.vcc/index` file. If the file matches a pattern in your `.vccignore`, it will securely skip staging that file.

---

### `write-tree`
Create a Tree object from the current staging area.

**Usage:**
```powershell
.\vcc.exe write-tree
```
**Details:**
Takes all the files currently staged in the index and writes a Tree object into `.vcc/objects`. It returns the generated 40-character SHA-1 hash of the tree. This is generally invoked behind the scenes during a commit but can be run manually for debugging.

---

### `commit`
Record changes to the repository.

**Usage:**
```powershell
.\vcc.exe commit "<commit message>"
```
**Details:**
Generates a new Tree from the index (via `write-tree`), assigns the current HEAD as the parent, and stores this snapshot along with your provided message as a Commit object. It outputs the new Commit SHA-1 hash and updates HEAD to point to this new commit.

---

### `log`
Display the commit history.

**Usage:**
```powershell
.\vcc.exe log
```
**Details:**
Traverses backward through the history of commits, starting from the current HEAD, and prints the commit hash, author information, and commit messages to the terminal.

---

### `checkout`
Restore working tree files from a specific commit.

**Usage:**
```powershell
.\vcc.exe checkout <commit-hash>
```
**Details:**
Looks up the Commit object by its SHA-1 hash, finds its associated Tree, and overwrites the files in your working directory to match the specific content from that point in time. It also automatically updates the HEAD reference to the checked-out commit.
