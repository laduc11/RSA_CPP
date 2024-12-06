#include <RSA.h>

vector<int> first_primes = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
                                    31, 37, 41, 43, 47, 53, 59, 61, 67,
                                    71, 73, 79, 83, 89, 97, 101};

// Utility function to do modular exponentiation.
void power(mpz_t rop, mpz_t x, mpz_t y, mpz_t n)
{
    mpz_mod(x, x, n);

    mpz_t temp;
    mpz_init(temp);
    mpz_set_ui(rop, 1);

    while (mpz_cmp_ui(y, 0))
    {
        if (mpz_mod_ui(temp, y, 2) == true)
        {
            mpz_mul(temp, rop, x);
            mpz_mod(rop, temp, n);
        }

        mpz_mul(temp, x, x);
        mpz_mod(x, temp, n);
        
        mpz_div_ui(y, y, 2);
    }
}


unsigned int getSeed(int entropy = 0)
{
    auto now = std::chrono::high_resolution_clock::now();
    unsigned seed = now.time_since_epoch().count() + entropy;
    return seed;
}

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

    mpz_add(rop, min , rop);

    // Ensure odd by setting lsb bit = 1
    mpz_setbit(rop, 0);
}


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
            return false;
        }
    }
    return true;
}

void millerTest(atomic_bool& roundPrimeCheck, mpz_t candidate, const int threadID)
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
            return;
        }

        r = r - 1;
    }

    // Clear the memory
    mpz_clears(decremented_candidate, round_tester, d, x, temp, nullptr);
    
    // Return composite
    roundPrimeCheck.store(false);
}

bool multiThreadMiller (mpz_t candidate, const int numThread)
{
    atomic_bool roundPrimeCheck(true);
    vector<thread> threads;

    for (int i = 0; i < numThread; i++)
    {
        threads.push_back(thread(millerTest, ref(roundPrimeCheck), candidate, i));
    } 

    for (auto& thread : threads)
    {
        thread.join();
    }

    return roundPrimeCheck;
}

void nBitsRandomPrime(mpz_t rop, mp_bitcnt_t number_of_bits, const int threadID, const int rounds, atomic_bool& primeFound)
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

#define NUM_THREAD 50
#define NUM_MILLER_ROUNDS 20

void generate_large_prime(mpz_t rop, mp_bitcnt_t number_of_bits) {
    vector<thread> threads;
    atomic_bool primeFound(false);


    for (int i = 0; i < NUM_THREAD; i++)
    {
        threads.push_back(thread(nBitsRandomPrime, rop, number_of_bits, i, NUM_MILLER_ROUNDS, ref(primeFound)));
    }

    for (auto& t : threads)
    {
        t.join();
    }
}

int main() {
    mpz_t largePrime;
    mpz_init(largePrime);

    generate_large_prime(largePrime, 2048);
}