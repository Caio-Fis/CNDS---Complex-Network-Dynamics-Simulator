# INSTALL.md

## Installation Guide for CNDS - Complex Network Dynamics Simulator

### Linux Ubuntu/Debian
1. **Update package list**:
   ```bash
   sudo apt update
   ```
2. **Install dependencies**:
   ```bash
   sudo apt install build-essential cmake g++ git
   ```
3. **Clone the repository**:
   ```bash
   git clone https://github.com/Caio-Fis/CNDS---Complex-Network-Dynamics-Simulator.git
   cd CNDS---Complex-Network-Dynamics-Simulator
   ```
4. **Build the project**:
   ```bash
   mkdir build
   cd build
   cmake ..
   make
   ```

### Linux Fedora/RHEL
1. **Update package list**:
   ```bash
   sudo dnf update
   ```
2. **Install dependencies**:
   ```bash
   sudo dnf install make automake gcc gcc-c++ git cmake
   ```
3. **Clone the repository**:
   ```bash
   git clone https://github.com/Caio-Fis/CNDS---Complex-Network-Dynamics-Simulator.git
   cd CNDS---Complex-Network-Dynamics-Simulator
   ```
4. **Build the project**:
   ```bash
   mkdir build
   cd build
   cmake ..
   make
   ```

### Windows MSYS2/MinGW
1. **Install MSYS2** from [https://www.msys2.org/](https://www.msys2.org/).
2. **Open MSYS2 terminal and update packages**:
   ```bash
   pacman -Syu
   ```
3. **Install dependencies**:
   ```bash
   pacman -S base-devel mingw-w64-x86_64-gcc git cmake
   ```
4. **Clone the repository**:
   ```bash
   git clone https://github.com/Caio-Fis/CNDS---Complex-Network-Dynamics-Simulator.git
   cd CNDS---Complex-Network-Dynamics-Simulator
   ```
5. **Build the project**:
   ```bash
   mkdir build
   cd build
   cmake ..
   make
   ```

### macOS
1. **Install Xcode** from the App Store.
2. **Install Homebrew** if not already installed:
   ```bash
   /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
   ```
3. **Install dependencies**:
   ```bash
   brew install cmake git
   ```
4. **Clone the repository**:
   ```bash
   git clone https://github.com/Caio-Fis/CNDS---Complex-Network-Dynamics-Simulator.git
   cd CNDS---Complex-Network-Dynamics-Simulator
   ```
5. **Build the project**:
   ```bash
   mkdir build
   cd build
   cmake ..
   make
   ```

### Troubleshooting
- If you encounter issues during installation, check the following:
  - Ensure all dependencies are correctly installed.
  - Check for error messages, and consult the documentation for specific tools if necessary.
  - For build issues, ensure you have the correct permissions to execute commands and write to directories.

For further assistance, consider reaching out to the community or checking the GitHub issues page for guidance.

---