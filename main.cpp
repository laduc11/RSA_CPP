#include <iostream>
#include <gmp.h>

int main()
{
    mpz_t a, b;
    mpz_init(a);
    mpz_init(b);

    mpz_set_ui(a, 1);
    mpz_set_ui(b, 2);

    unsigned int n;
    std::cout << "Enter n = ";
    std::cin >> n;


    // Base case
    if (n == 0 || n == 1) {
        std::cout << "Fibonacci first = " << 1 << std::endl;
        return 0;
    } else if (n == 2) {
        std::cout << "Fibonacci second = " << 2 << std::endl;
        return 0;
    }

    // Calcaulate fibonacci
    for (unsigned int i = 2; i < n; ++i) {
        mpz_add(a, a, b);
        mpz_swap(a, b);
    }

    std::cout << "Fibonacci " << n << "th = " << b << std::endl;

    return 0;
}