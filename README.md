# Chat Project
A simple chat application based on socket connections.

## 📚 Overview
This is a multi-threaded chat system project written in modern C++.
It consists of three core modules:

- **lib/**: Common library, including external dependencies and utilities (such as the logger).
- **client/**: Client-side implementation, communicates with the server via sockets.
- **server/**: Server-side implementation, handles multiple clients and logs messages.

This project is primarily for practice purposes, focusing on C++ networking, multithreading, and fundamental project architecture design.

---

## ✨ Features

- **Terminal UI**: Built with the FTXUI library for a user-friendly interface in the terminal.
- **Simple login system**: Uses username and password to identify users.
- **Friend management**: Add friends, accept/reject requests, and remove friends.
- **One-on-one chat**: Select a friend and exchange text messages.
- **Chat history**: Stores conversation logs and uses lazy loading for efficient retrieval.

---

## 🚀 Build Instructions

### 📦 Prerequisites
- C++17 compiler (e.g. GCC 9+, Clang, MSVC 2019+)
- CMake >= 3.15
- Make or Ninja

### ⚙️ Build Steps

#### 1️⃣ Clone this repository

**Recommended (SSH):**
```bash
git clone git@github.com:Bongo044152/little_chat_project.git
````

**Or HTTPS:**

```bash
git clone https://github.com/Bongo044152/little_chat_project.git
```

#### 2️⃣ Prepare the build directory

```bash
cd little_chat_project
mkdir build && cd build
```

#### 3️⃣ Run CMake

If using MinGW on Windows (recommended):

```bash
cmake -G "MinGW Makefiles" ..
cmake --build .
```

For Linux/macOS (or WSL), simply:

```bash
cmake ..
make
```

---

## 📂 Project Structure

```
project/
├── lib/       # Core utilities and shared modules
├── client/    # Client implementation
├── server/    # Server implementation
├── test/      # Unit tests (doctest)
├── docs/      # documentations
└── CMakeLists.txt
```
