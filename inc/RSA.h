#ifndef RSA_H
#define RSA_H

#include "main.h"

class RSA {
private:
    mpz_t p, q, e, d, n;

public:
    RSA();
    void calcGCD(mpz_t result, const mpz_t first, const mpz_t second);
};

#endif // RSA_H