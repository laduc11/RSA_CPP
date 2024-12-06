#include "RSA.h"

RSA::RSA() {
    mpz_inits(p, q, e, d, n, NULL);
}

RSA::RSA(int p, int q, int e) {
    mpz_inits(this->p, this->q, this->e, this->d, this->n, NULL);
    mpz_set_ui(this->p, p);
    mpz_set_ui(this->q, q);
    mpz_set_ui(this->e, e);
    mpz_init(d);
    calcd();
    mpz_mul(this->n, this->p, this->q);
}

/**
 * @brief Caculate Greatest Division Common
 * 
 * @param result 
 * @param first 
 * @param second 
 */
void RSA::calcGCD(mpz_t result, const mpz_t first, const mpz_t second) {
    mpz_t num_1, num_2;
    mpz_init_set(num_1, first);
    mpz_init_set(num_2, second);

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
    }
    mpz_set(result, num_1);
}

/**
 * @brief Calculate d from e and phi(n) in RSA algorithm
 * 
 * @param d 
 * @param e 
 * @param phi 
 */
void RSA::calcd() {
    // s: sequence
    // r: remainder
    // e * x + phi(n) * y = gcd(e, phi(n))

    // Initialize
    mpz_t s, old_s, r, old_r, x, y;
    // Calculate phi(n) = (p - 1)(q - 1)
    mpz_t phi, temp_phi;
    mpz_inits(phi, temp_phi, NULL);
    mpz_sub_ui(phi, p, 1);
    mpz_sub_ui(temp_phi, q, 1);
    mpz_mul(phi, phi, temp_phi);

    mpz_init_set_ui(s, 0);
    mpz_init_set_ui(old_s, 1);
    mpz_init_set(r, phi);
    mpz_init_set(old_r, e);

    while (mpz_cmp_ui(r, 0)) {
        mpz_t q, temp;
        mpz_init(q);
        mpz_div(q, old_r, r);
        mpz_init_set(temp, r);

        // r = old_r - qr
        mpz_mul(r, r, q);
        mpz_sub(r, old_r, r);
        // old_r = r
        mpz_set(old_r, temp);

        mpz_set(temp, s);
        // s = old_s - qs
        mpz_mul(s, s, q);
        mpz_sub(s, old_s, s);
        // old_s = s
        mpz_set(old_s, temp);
    }

    if (mpz_cmp_ui(phi, 0)) {
        mpz_init(y);
        mpz_mul(y, old_s, e);
        mpz_sub(y, old_r, y);
        mpz_div(y, y, phi);
    } else {
        mpz_init_set_ui(y, 0);
    }
    mpz_init_set(x, old_s);
    if (mpz_cmp_ui(x, 0) < 0) {
        mpz_add(d, phi, x);
    } else {
        mpz_set(d, x);
    }
}
