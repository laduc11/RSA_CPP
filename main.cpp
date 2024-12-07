#include "main.h"
#include "RSA.h"

int main()
{
    RSA rsa;
    rsa.setFilePlaintext("plaintext.txt");
    rsa.setFilePublicKey("public.key");
    rsa.setFilePrivateKey("private.key");
    rsa.generateKeyPair(2048);
    rsa.encrypt();
    rsa.decrypt();
    return 0;
}