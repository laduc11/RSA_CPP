#include "utils.h"

std::vector<int> first_primes = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
                                 31, 37, 41, 43, 47, 53, 59, 61, 67,
                                 71, 73, 79, 83, 89, 97, 101, 103};

/**
 * @brief Caculate Greatest Division Common
 * 
 * @param result 
 * @param first 
 * @param second 
 */
void calcGCD(mpz_t result, const mpz_t first, const mpz_t second) {
    mpz_t num_1, num_2;
    mpz_init_set(num_1, first);
    mpz_init_set(num_2, second);
    if (mpz_cmp(num_1, num_2) < 0) {
        mpz_swap(num_1, num_2);
    }

    // Euler algorithm non-recursion
    while (mpz_cmp(num_1, num_2)) {
        // Handle exception GCD with zero
        if (mpz_cmp_ui(num_1, 0) == 0) {
            mpz_set(result, num_2);
            return;
        } else if (mpz_cmp_ui(num_2, 0) == 0) {
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

    // release memory
    mpz_clears(num_1, num_2, nullptr);
}

/**
 * @brief Utility function to do modular exponentiation.
 * expression: rop = (x ^ y) mod n
 * 
 * @param rop result operand
 * @param x 
 * @param y 
 * @param n 
 */
void power(mpz_t rop, const mpz_t x, const mpz_t y, const mpz_t n)
{
    // copy parameter
    mpz_t temp_x, temp_y;
    mpz_init_set(temp_x, x);
    mpz_init_set(temp_y, y);

    mpz_mod(temp_x, temp_x, n);

    mpz_t temp;
    mpz_init(temp);
    mpz_set_ui(rop, 1);

    while (mpz_cmp_ui(temp_y, 0))
    {
        if (mpz_mod_ui(temp, temp_y, 2) == true)
        {
            mpz_mul(temp, rop, temp_x);
            mpz_mod(rop, temp, n);
        }

        mpz_mul(temp, temp_x, temp_x);
        mpz_mod(temp_x, temp, n);

        mpz_div_ui(temp_y, temp_y, 2);
    }

    // release memory
    mpz_clears(temp, temp_x, temp_y, nullptr);
}

/**
 * @brief Get the Seed for random algorithm
 * 
 * @param entropy 
 * @return unsigned int 
 */
unsigned int getSeed(int entropy)
{
    auto now = std::chrono::high_resolution_clock::now();
    unsigned seed = now.time_since_epoch().count() + entropy;
    return seed;
}

/**
 * @brief Generate a n-bits prime number with multi-threads
 * 
 * @param rop 
 * @param number_of_bits 
 */
void generate_large_prime(mpz_t rop, mp_bitcnt_t number_of_bits)
{
    std::vector<std::thread> threads;
    std::atomic_bool primeFound(false);

    for (int i = 0; i < NUM_THREAD; i++)
    {
        threads.push_back(std::thread(nBitsRandomPrime, rop, number_of_bits, i, NUM_MILLER_ROUNDS, std::ref(primeFound)));
    }

    for (auto &t : threads)
    {
        t.join();
    }
}

/**
 * @brief Simple check prime number
 *
 * @param candidate
 * @return true or fasle
 */
bool lowLevelPrimalityCheck(mpz_t candidate)
{
    mpz_t isDivisible;
    mpz_init(isDivisible);

    for (int prime : first_primes)
    {
        mpz_mod_ui(isDivisible, candidate, prime);
        // return false if candidate is divisible by small prime numbers
        if (!mpz_cmp_ui(isDivisible, 0))
        {
            mpz_clear(isDivisible);
            return false;
        }
    }
    mpz_clear(isDivisible);
    return true;
}

/**
 * @brief Random an odd n-bits number
 * 
 * @param rop 
 * @param number_of_bits 
 * @param entropy 
 */
void nBitsRandomOddNum(mpz_t rop, mp_bitcnt_t number_of_bits, const int entropy = 0)
{
    gmp_randstate_t random_state;
    gmp_randinit_mt(random_state);

    gmp_randseed_ui(random_state, getSeed(entropy));

    // Generate random number from 0 to 2^(n-1)
    mpz_urandomb(rop, random_state, number_of_bits - 1);

    mpz_t min;
    mpz_init_set_ui(min, 1);
    mpz_mul_2exp(min, min, number_of_bits - 1);

    mpz_add(rop, min, rop);

    // Ensure odd by setting lsb bit = 1
    mpz_setbit(rop, 0);
}

/**
 * @brief Generate n-bits prime number (Callback function for thread)
 * 
 * @param rop 
 * @param number_of_bits 
 * @param threadID 
 * @param rounds 
 * @param primeFound 
 */
void nBitsRandomPrime(mpz_t rop, mp_bitcnt_t number_of_bits, const int threadID, const int rounds, atomic_bool &primeFound)
{
    mpz_t random_number;
    mpz_init(random_number);

    while (!primeFound)
    {
        nBitsRandomOddNum(random_number, number_of_bits, threadID);
        while (!lowLevelPrimalityCheck(random_number))
        {
            nBitsRandomOddNum(random_number, number_of_bits, threadID);
        }

        if (multiThreadMiller(random_number, rounds) && !primeFound)
        {
            mpz_set(rop, random_number);
            mpz_clear(random_number);
            primeFound.store(true);
        }
    }
}

/**
 * @brief Run and manage multi-miller thread
 * 
 * @param candidate 
 * @param numThread 
 * @return true 
 * @return false 
 */
bool multiThreadMiller(mpz_t candidate, const int numThread)
{
    std::atomic_bool roundPrimeCheck(true);
    std::vector<std::thread> threads;

    for (int i = 0; i < numThread; i++)
    {
        threads.push_back(std::thread(millerTest, std::ref(roundPrimeCheck), candidate, i));
    }

    for (auto &thread : threads)
    {
        thread.join();
    }

    return roundPrimeCheck;
}

/**
 * @brief 
 * 
 * @param roundPrimeCheck 
 * @param candidate 
 * @param threadID 
 */
void millerTest(std::atomic_bool &roundPrimeCheck, mpz_t candidate, const int threadID)
{
    // Initialize state for a Mersenne Twister algorithm
    gmp_randstate_t random_state;
    gmp_randinit_mt(random_state);

    gmp_randseed_ui(random_state, getSeed(threadID));

    mpz_t decremented_candidate;
    mpz_init(decremented_candidate);

    // Get a random number used for test in range [2, candidate - 2]
    mpz_t round_tester;
    mpz_init(round_tester);
    mpz_sub_ui(decremented_candidate, candidate, 4);
    // First get random number in range [0, candidate - 4]
    mpz_urandomm(round_tester, random_state, decremented_candidate);
    // Then + 2 the number to get 2 <= round_tester <= candidate - 2
    mpz_add_ui(round_tester, round_tester, 2);

    // Find d and r so that d*(2^r) = n - 1
    unsigned long int r = 0;
    mpz_t d;
    mpz_t x;
    mpz_t temp;

    mpz_init(x);
    mpz_init(temp);
    mpz_sub_ui(temp, candidate, 1);
    mpz_init_set(d, temp);

    while (mpz_mod_ui(temp, d, 2) == 0)
    {
        mpz_div_ui(d, d, 2);
        r = r + 1;
    }

    // x = (round_tester ^ d) % candidate
    power(x, round_tester, d, candidate);
    // (x == 1 or x == candidate - 1)
    mpz_sub_ui(decremented_candidate, candidate, 1);
    if (!mpz_cmp_ui(x, 1) || !mpz_cmp(x, decremented_candidate))
    {
        // Clear the memory
        mpz_clears(decremented_candidate, round_tester, d, x, temp, nullptr);
        return;
    }

    while (r > 0)
    {
        // x = (x ^ x) mod candidate
        power(x, x, x, candidate);

        // (x == 1)
        if (!mpz_cmp_ui(x, 1))
        {
            // Return composite
            roundPrimeCheck.store(false);
        }

        // (x == n - 1)
        if (!mpz_cmp(x, decremented_candidate))
        {
            // Clear the memory
            mpz_clears(decremented_candidate, round_tester, d, x, temp, nullptr);
            return;
        }

        r = r - 1;
    }

    // Clear the memory
    mpz_clears(decremented_candidate, round_tester, d, x, temp, nullptr);

    // Return composite
    roundPrimeCheck.store(false);
}

/**
 * @brief Import data like key or plaintext to mpz_t instance
 * 
 * @param des 
 * @param filename 
 */
void importDataFromFile(mpz_t des, std::string filename)
{
    std::fstream file(filename, std::ios::in | std::ios::binary | std::ios::ate);
    // Handle case open file fail
    if (!file) {
        std::cout << "Open file " << filename << " fail" << std::endl;
        return;
    }
    // Get size of file and move cursor to the begining of file
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    uint8_t *buffer = new uint8_t[size];

    if (file.read((char *)buffer, size)) {
        // Read data success
        mpz_import(des, (size_t)size, 1, sizeof(uint8_t), 0, 0, buffer);
        std::cout << "Read data successfully" << std::endl;
        mpz_out_str(stdout, 16, des);
        std::cout << std::endl;
        char *output = mpz_get_str(nullptr, 16, des);

        std::string plaintext;
        for (size_t i = 0; output[i] != '\0'; i += 2) {
            // Convert each pair of hex digits to a character
            std::string hexByte(output + i, 2);
            char asciiChar = static_cast<char>(std::stoi(hexByte, nullptr, 16));
            plaintext += asciiChar;
        }
        std::cout << "plaintext: " << plaintext << std::endl;
    }
    else
    {
        std::cout << "Cannot reead file " << filename << std::endl;
    }

    delete[] buffer;
}
