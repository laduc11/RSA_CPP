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

int main()
{
    // excepted d = 399553909
    RSA rsa(18191, 40169, 156453389);
    return 0;
}