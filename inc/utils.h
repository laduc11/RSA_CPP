#ifndef UTILS_H
#define UTILS_H

#include "main.h"

#define NUM_THREAD          50
#define NUM_MILLER_ROUNDS   20

void calcGCD(mpz_t result, const mpz_t first, const mpz_t second);
void readDataFromFile(mpz_t des, std::string filename);
void writeFileFromGMPvariable(mpz_t src, std::string filename);
void power(mpz_t rop, const mpz_t x, const mpz_t y, const mpz_t n);

// Generate random prime number
unsigned int getSeed(int entropy = 0);
void generate_large_prime(mpz_t rop, mp_bitcnt_t number_of_bits);
bool lowLevelPrimalityCheck(mpz_t candidate);
void nBitsRandomOddNum(mpz_t rop, mp_bitcnt_t number_of_bits, const int entropy = 0);

// Threading
bool multiThreadMiller(mpz_t candidate, const int numThread);
void millerTest(std::atomic_bool &roundPrimeCheck, mpz_t candidate, const int threadID);
void nBitsRandomPrime(mpz_t rop, mp_bitcnt_t number_of_bits, const int threadID, const int rounds, std::atomic_bool &primeFound);


#endif  // UTILS_H