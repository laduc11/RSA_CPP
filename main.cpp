#include "main.h"
#include "RSA.h"

int main()
{
    RSA rsa;
    rsa.setFilePlaintext("test1.txt");
    rsa.setFilePublicKey("public1.key");
    rsa.setFilePrivateKey("private1.key");
    auto now = std::chrono::high_resolution_clock::now();
    rsa.generateKeyPair(512);
    std::cout << "time to gen 512 bit key: " << std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - now).count() << " us" << std::endl;
    now = std::chrono::high_resolution_clock::now();
    rsa.encrypt();
    std::cout << "time to encrypt test1: " << std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - now).count() << " us" << std::endl;
    rsa.decrypt();
    return 0;
}