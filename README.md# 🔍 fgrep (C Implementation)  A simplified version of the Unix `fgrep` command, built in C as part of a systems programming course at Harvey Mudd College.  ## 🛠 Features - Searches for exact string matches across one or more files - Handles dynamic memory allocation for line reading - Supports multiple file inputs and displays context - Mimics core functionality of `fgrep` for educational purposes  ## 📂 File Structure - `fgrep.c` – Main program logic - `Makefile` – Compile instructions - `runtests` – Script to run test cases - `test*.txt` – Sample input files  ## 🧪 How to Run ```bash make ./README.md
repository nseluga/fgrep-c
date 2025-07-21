# 🔍 fgrep (C Implementation)

A simplified version of the Unix `fgrep` command, built in C as part of a systems programming course at Harvey Mudd College.

## 🛠 Features
- Searches for exact string matches across one or more files
- Handles dynamic memory allocation for line reading
- Supports multiple file inputs and displays context
- Mimics core functionality of `fgrep` for educational purposes

## 📂 File Structure
- `fgrep.c` – Main program logic
- `Makefile` – Compile instructions
- `runtests` – Script to run test cases
- `test*.txt` – Sample input files

## 🧪 How to Run
```bash
make
./fgrep [pattern] [file1] [file2] ...
