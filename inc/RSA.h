#ifndef RSA_H
#define RSA_H

#include "main.h"

class RSA {
private:
    mpz_t p, q, e, d, n;

public:
    RSA();
    RSA(int p, int q, int e);
    void calcGCD(mpz_t result, const mpz_t first, const mpz_t second);
    void calcd();
};

#endif // RSA_H