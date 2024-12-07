#ifndef RSA_H
#define RSA_H

#include "main.h"
#include "utils.h"

#define ENCODE_EXT  ".enc"
#define DECODE_EXT  ".dec"
#define KEY_EXT     ".key"

class RSA {
private:
    mpz_t p, q, e, d, n;
    std::string filePublicKey, filePrivateKey, filePlaintext;

    void generateKey(mpz_t public_key, mpz_t private_key, mp_bitcnt_t number_of_bits);
    void storeKey(const mpz_t public_key, const mpz_t private_key);
    void initE();
    void calcD();
    bool verified(std::string msgFile, std::string decFile);

public:
    RSA();
    RSA(int p, int q, int e);

    void generateKeyPair();
    void setFilePublicKey(std::string filename);
    void setFilePrivateKey(std::string filename);
    void setFilePlaintext(std::string filename);
    void setE(mpz_t e);

    void encrypt();
    void decrypt();
};

#endif // RSA_H