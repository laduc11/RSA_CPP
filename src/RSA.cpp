#include "RSA.h"

RSA::RSA() {
    mpz_init(p);
    mpz_init(q);
    mpz_init(e);
    mpz_init(d);
    mpz_init(n);
}
