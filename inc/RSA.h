#ifndef RSA_H
#define RSA_H

#include "main.h"

class RSA {
private:
    mpz_t p, q, e, d, n;

public:
    RSA();
    RSA(int p, int q, int e);
    void calcd();
};

// Utils function;
void calcGCD(mpz_t result, const mpz_t first, const mpz_t second);
void importDataFromFile(mpz_t des, std::string filename);

#endif // RSA_H