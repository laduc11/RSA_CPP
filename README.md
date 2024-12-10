# Installation GMP
## Windows

### Prerequisites

- A Windows 10/11 operating system (64-bit recommended).
- Administrator privileges on your system.


### Steps to Install GMP library

#### 1. Download MSYS2 Installer

1. Visit the [official MSYS2 website](https://www.msys2.org/).
2. Download the **MSYS2 installer** for Windows.

#### 2. Run the Installer

1. Locate the downloaded installer (e.g., `msys2-x86_64-<version>.exe`).
2. Double-click the installer to launch it.
3. Follow the on-screen instructions:
   - **Installation Directory**: Choose or confirm the default directory (e.g., `C:\msys64`).
   - Proceed with the installation.

#### 3. Update the Package Database and install GCC

1. Launch MSYS2 UCRT64 by double-clicking the **MSYS2 MSYS UCRT64** shortcut.
2. Update the package database and install gcc by running the following commands in the terminal:

   ```bash
   pacman -Syu
   pacman  -S  mingw-w64-ucrt-x86_64-gcc
#### 4. Install GMP library
Run the following command in the terminal

   ```bash
   pacman -S mingw-w64-ucrt-x86_64-gmp
   ```
#### 5. Add MSYS2 Paths: 
In the **Edit Environment Variable** window, click **New** and add the following paths (adjust if your installation directory differs):
- `C:\msys64\ucrt64` 
- `C:\msys64\ucrt64\bin` 

# Setup Json file and build project in VS Code for Window
## Step to set
1. Firstly, config file **c_pp_properties.json**. Change parameter `compilerPath` to MSYS workspace that intall above and windowSdkVersion.
   ```json
    windowsSdkVersion": "10.0.19041.0
    compilerPath": "C:\\msys64\\ucrt64\\bin\\g++.exe

2. Nextly, add this code below to file **launch.json**. 
    ```json
    "configurations": [
    {
        "name": "(Windows) Launch",
        "type": "cppvsdbg",
        "request": "launch",
        "program": "test.exe", // file exe to run
        "args": ["300"], // this is optional
        "stopAtEntry": false,
        "cwd": "${fileDirname}",
        "environment": [],
        "console": "externalTerminal"
    }]

3. Thirdly, edit file **tasks.json**, this is the most important json file to build project with GMP library in Window. This file is too long to show, I just show some essential parameters:
    ```json
    "label": "C/C++: g++.exe build active file",
	"command": "C:\\msys64\\ucrt64\\bin\\g++.exe",
    "args": [
		"-fdiagnostics-color=always",
		"-g",
		"${file}",
		"-o",
		"${fileDirname}\\${fileBasenameNoExtension}.exe",
		"-lgmp",
		"-lgmpxx"
	],
    "detail": "compiler: C:\\msys64\\ucrt64\\bin\\g++.exe"
4. After editing 3 json above, in VS Code, you just press `Ctrl + Shift + B` on keyboard to build, the result if the building process successful is the file `.exe` was generated.
5. Finally, run file `.exe` in your workspace.

## Linux
Installing GMP in Linux is more simple. Just run command below
```bash
sudo apt-get install limgmp3-dev libgmp10
```

# Compile and run project

## Set up in Ubuntu
### 1. Build project
```bash
make all
```
### 2. Run project
```bash
make run
```
## Set up in Window
### 1. Install chocolatey
Run Powershell as adminstrator and run below command
```bash
Set-ExecutionPolicy Bypass -Scope Process -Force; [System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]::SecurityProtocol -bor 3072; iex ((New-Object System.Net.WebClient).DownloadString('https://community.chocolatey.org/install.ps1'))
```
### 2. Install make in Window
```bash
choco install make
```

## Compile and run manually
Compile command:
```bash
g++ source_file.cpp -o main -lgmp -lgmpxx -I.
```
Run command:
```bash
./main
```