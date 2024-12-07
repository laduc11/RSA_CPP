# RSA_CPP

## Set up in Ubuntu
### 1. Create folder `build`
```bash
mkdir -p build
cd build
```
### 2. Build sample program
```bash
cmake ..
make -j
```
### 3. Run program
```bash
./main
```
## Set up in Window
### 1. Install chocolatey
Run Powershell as adminstrator and run below command
```bash
Set-ExecutionPolicy Bypass -Scope Process -Force; [System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]::SecurityProtocol -bor 3072; iex ((New-Object System.Net.WebClient).DownloadString('https://community.chocolatey.org/install.ps1'))
```
### 2. Install cmake and make in Window
```bash
choco install make cmake
```
After that, set up `build` dictionary and run like Ubuntu

## Compile and run manually
Compile command:
```bash
g++ source_file.cpp -o main -lgmp -I.
```
Run command:
```bash
./main
```