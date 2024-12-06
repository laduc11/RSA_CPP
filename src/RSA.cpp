#include "RSA.h"

RSA::RSA() {
    mpz_init(p);
    mpz_init(q);
    mpz_init(e);
    mpz_init(d);
    mpz_init(n);
}

void RSA::calcGCD(mpz_t result, mpz_t first, mpz_t second) {
    mpz_t num_1, num_2;
    mpz_init_set(num_1, first);
    mpz_init_set(num_2, second);
    std::cout << "first:  " << num_1 << std::endl;
    std::cout << "second: " << num_2 << std::endl;

    // Euler algorithm non-recursion
    while (mpz_cmp(num_1, num_2)) {
        // Handle exception GCD with zero
        if (!mpz_cmp_ui(num_1, 0)) {
            mpz_set(result, num_2);
            return;
        } else if (!mpz_cmp_ui(num_2, 0)) {
            mpz_set(result, num_1);
            return;
        }
        // Main code block
        if (mpz_cmp(num_1, num_2) > 0) {
            mpz_mod(num_1, num_1, num_2);
        } else {
            mpz_mod(num_2, num_2, num_1);
        }
        std::cout << "first:  " << num_1 << std::endl;
        std::cout << "second: " << num_2 << std::endl;
    }
    mpz_set(result, num_1);
}
