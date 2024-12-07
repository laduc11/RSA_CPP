#include "main.h"
#include "RSA.h"

void calcFibo(uint32_t n) {
    mpz_t a, b;
    mpz_init(a);
    mpz_init(b);

    mpz_set_ui(a, 1);
    mpz_set_ui(b, 2);

    // Base case
    if (n == 0 || n == 1) {
        std::cout << "Fibonacci 1st = " << 1 << std::endl;
        return;
    }
    else if (n == 2) {
        std::cout << "Fibonacci 2nd = " << 2 << std::endl;
        return;
    }

    // Calcaulate fibonacci
    for (unsigned int i = 2; i < n; ++i) {
        mpz_add(a, a, b);
        mpz_swap(a, b);
    }

    std::cout << "Fibonacci " << n << "th = " << b << std::endl;
}

#include <iostream>
#include <fstream>
#include <vector>

void testReadFile() {
    std::ifstream file("myfile.txt", std::ios::binary | std::ios::ate); // Open file in binary mode and seek to the end
    if (!file) {
        std::cerr << "Error opening file." << std::endl;
        return;
    }

    std::streamsize size = file.tellg(); // Get the file size
    file.seekg(0, std::ios::beg);       // Seek back to the beginning

    std::vector<uint8_t> buffer(size);  // Create a vector of appropriate size
    if (file.read(reinterpret_cast<char*>(buffer.data()), size)) {
        // Successfully read the file
        std::cout << "File read successfully. Data size: " << buffer.size() << " bytes." << std::endl;

        // Example: Print the bytes
        for (uint8_t byte : buffer) {
            std::cout << static_cast<char>(byte) << " ";
        }
        std::cout << std::endl;
    } else {
        std::cerr << "Error reading file." << std::endl;
    }
}


int main()
{
    mpz_t data;
    mpz_init(data);
    importDataFromFile(data, "../myfile.txt");
    return 0;
    // excepted d = 399553909
    RSA rsa(18191, 40169, 156453389);
    return 0;
}