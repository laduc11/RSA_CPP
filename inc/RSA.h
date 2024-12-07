#ifndef RSA_H
#define RSA_H

#include "main.h"
#include "utils.h"

#define ENCODE_EXT  ".enc"
#define DECODE_EXT  ".dec"
#define KEY_EXT     ".key"

#define PLAINTEXT_DIR   "plaintext/"
#define CIPHERTEXT_DIR  "ciphertext/"
#define KEY_DIR         "key/"
#define DECRYPTED_DIR   "decrypted/"

class RSA {
private:
    mpz_t p, q, e, d, n;
    std::string filePublicKey, filePrivateKey, filePlaintext, fileDecrypted;

    void storeKey();
    void initE();
    void calcD();
    bool verified(std::string msgFile, std::string decFile);

public:
    RSA();
    RSA(int p, int q, int e);

    void generateKeyPair(mp_bitcnt_t number_of_bits);
    void setFilePublicKey(std::string filename);
    void setFilePrivateKey(std::string filename);
    void setFilePlaintext(std::string filename);
    void setE(mpz_t e);

    void encrypt();
    void decrypt();
};

#endif // RSA_H