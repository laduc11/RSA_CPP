#include "RSA.h"

RSA::RSA() {
    mpz_inits(p, q, e, d, n, NULL);
    filePublicKey = "pub.key";
    filePrivateKey = "prv.key";
    initE();
    calcD();
}

RSA::RSA(int p, int q, int e) {
    mpz_inits(this->p, this->q, this->e, this->d, this->n, NULL);
    mpz_set_ui(this->p, p);
    mpz_set_ui(this->q, q);
    mpz_set_ui(this->e, e);
    mpz_init(d);
    calcD();
    mpz_mul(this->n, this->p, this->q);
}

/**
 * @brief Initialize parameter e
 * 
 */
void RSA::initE()
{
    mpz_set_ui(e, 65537);
}

/**
 * @brief Calculate d from e and phi(n) in RSA algorithm
 * 
 */
void RSA::calcD() {
    // s: sequence
    // r: remainder
    // e * x + phi(n) * y = gcd(e, phi(n))

    // Calculate phi(n) = (p - 1)(q - 1)
    mpz_t phi, temp_phi;
    mpz_inits(phi, temp_phi, NULL);
    mpz_sub_ui(phi, p, 1);
    mpz_sub_ui(temp_phi, q, 1);
    mpz_mul(phi, phi, temp_phi);

    // Initialize
    mpz_t s, old_s, r, old_r, x, y;
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

    // release memory
    mpz_clears(s, old_s, r, old_r, x, y, temp_phi, phi, nullptr);
}

// Setter fuinctions
void RSA::setFilePublicKey(std::string filename)
{
    this->filePublicKey = filename;
}
void RSA::setFilePrivateKey(std::string filename)
{
    this->filePrivateKey = filename;
}
void RSA::setFilePlaintext(std::string filename)
{
    this->filePlaintext = filename;
}
void RSA::setE(mpz_t e)
{
    mpz_set(this->e, e);
}

/**
 * @brief Generate n-bits public key and private key
 * 
 * @param number_of_bits 
 */
void RSA::generateKeyPair(mp_bitcnt_t number_of_bits)
{
    std::thread t1 = std::thread(generateLargePrime, this->p, number_of_bits / 2);
    std::thread t2 = std::thread(generateLargePrime, this->q, number_of_bits / 2);
    t1.join();
    t2.join();

    std::cout << "p = ";
    mpz_out_str(stdout, 10, this->p);
    std::cout << std::endl;

    std::cout << "q = ";
    mpz_out_str(stdout, 10, this->q);
    std::cout << std::endl;

    mpz_t phi_n;
    mpz_init(phi_n);

    // calculate n = p * q
    mpz_mul(this->n, this->p, this->q);

    // calculate phi(n) = (p - 1)(q - 1)
    mpz_t p_minus_1, q_minus_1;
    mpz_inits(p_minus_1, q_minus_1, nullptr);
    mpz_sub_ui(p_minus_1, this->p, 1);
    mpz_sub_ui(q_minus_1, this->q, 1);
    mpz_mul(phi_n, p_minus_1, q_minus_1);

    // CHECK e AND n PRIME TOGETHER
    mpz_t check_gcd;
    mpz_init(check_gcd);
    calcGCD(check_gcd, phi_n, this->e);
    while (true)
    {
        if (mpz_cmp_si(check_gcd, 1) == 0)
            break;
        std::thread t1 = std::thread(generateLargePrime, this->p, number_of_bits / 2);
        std::thread t2 = std::thread(generateLargePrime, this->q, number_of_bits / 2);
        t1.join();
        t2.join();

        // CALCULATE n
        mpz_mul(this->n, this->p, this->q);
        // CALCULATE PHI(N)
        mpz_sub_ui(p_minus_1, this->p, 1);
        mpz_sub_ui(q_minus_1, this->q, 1);
        mpz_mul(phi_n, p_minus_1, q_minus_1);

        // CHECK e AND n PRIME TOGETHER
        calcGCD(check_gcd, phi_n, this->e);
    }
    calcD();
    std::cout << "Store Public Key in file pub.key\n";
    storeKey();

    mpz_clears(phi_n, p_minus_1, q_minus_1, check_gcd, nullptr);
}

/**
 * @brief Write public key and private key to file
 * 
 */
void RSA::storeKey()
{
    std::ofstream pub(KEY_DIR + this->filePublicKey), pvt(KEY_DIR + this->filePrivateKey);
    std::cout << "Public key is stored in file "<< KEY_DIR + this->filePublicKey << "...\n";
    std::string n_str = mpz_get_str(nullptr, 16, this->n);
    std::string d_str = mpz_get_str(nullptr, 16, this->d);
    std::string e_str = mpz_get_str(nullptr, 16, this->e);
    if (pub.is_open())
    {
        pub << "Public key:\n";
        pub << e_str << '\n';
        pub << "n:\n";
        pub << n_str;
    }
    std::cout << "Private key is stored in file " << KEY_DIR + this->filePrivateKey << "...\n";
    if (pvt.is_open())
    {
        pvt << "Private key:\n";
        pvt << d_str << '\n';
        pvt << "n:\n";
        pvt << n_str;
    }
}

/**
 * @brief Compare Decrypted and original Message
 * 
 * @param msgFile 
 * @param decFile 
 * @return true 
 * @return false 
 */
bool RSA::verified(std::string msgFile, std::string decFile)
{
    std::ifstream msg(msgFile), dec(decFile);
    if (!msg.is_open() || !dec.is_open())
        std::cerr << "Cannot open message file or decrypted message file.\n";
    std::vector<char> bf1((std::istreambuf_iterator<char>(msg)), std::istreambuf_iterator<char>());
    std::vector<char> bf2((std::istreambuf_iterator<char>(dec)), std::istreambuf_iterator<char>());
    msg.close();
    dec.close();
    return bf1 == bf2;
}

/**
 * @brief Encrypt message and save it to file .enc
 * 
 */
void RSA::encrypt()
{
    std::string public_key_file = KEY_DIR + this->filePublicKey, message_file = PLAINTEXT_DIR + this->filePlaintext;
    std::cout << "public file: " << public_key_file << std::endl;

    std::ifstream in_key(public_key_file), in_msg(message_file);
    if (!in_key.is_open())
    {
        std::cerr << "Error open Public Key file.\n";
        return;
    }

    std::string line;
    mpz_t pubkey, n;
    mpz_inits(pubkey, n, nullptr);
    while (getline(in_key, line))
    {
        if (line == "Public key:")
        {
            if (getline(in_key, line))
            {
                mpz_set_str(pubkey, line.c_str(), 16);
            }
            else
            {
                std::cerr << "Error reading public key value.\n";
                return;
            }
        }

        else if (line == "n:")
        {
            if (getline(in_key, line))
            {
                mpz_set_str(n, line.c_str(), 16);
            }
            else
            {
                std::cerr << "Error reading n value.\n";
                return;
            }
        }
    }

    // Read the message from file and convert it to an integer
    std::string msg;
    std::stringstream ss;

    // Show content of file
    std::cout << "Message: " << std::endl;
#if defined(_WIN32) || defined(_WIN64)
    std::string show_content_cmd = "type " + message_file;
    system(show_content_cmd.c_str());
#elif defined(__linux__)
    std::string show_content_cmd = "cat " + message_file;
    system(show_content_cmd.c_str());
#endif

    mpz_t rop, x;
    mpz_inits(rop, x, nullptr);

    readDataFromFile(x, message_file);

    std::cout << "msg (mpz_t) = ";
    mpz_out_str(stdout, 10, x);
    std::cout << std::endl;

    // Thực hiện mã hóa RSA: rop = (x^pubkey) % n
    power(rop, x, pubkey, n);

    // STORE ROP TO .ENC FILE
    std::string result = mpz_get_str(nullptr, 10, rop);
    std::cout << "Encrypt message: " << result << std::endl;
    std::string msgFile = this->filePlaintext;
    std::string encFile = msgFile.replace(msgFile.find('.'), 4, ENCODE_EXT);

    std::ofstream out(CIPHERTEXT_DIR + encFile);
    if (out.is_open())
    {
        out << result;
    }
    out.close();
    in_key.close();
    in_msg.close();
    return;
}

/**
 * @brief Decrypted ciphertext, save value after decrypting to file
 * 
 */
void RSA::decrypt()
{
    std::string msgFile = this->filePlaintext;
    std::string encFile = CIPHERTEXT_DIR + msgFile.replace(msgFile.find('.'), 4, ENCODE_EXT);
    std::string pvtFilename = KEY_DIR + this->filePrivateKey;
    std::cout << "private file: " << pvtFilename << std::endl;
    std::cout << "encFile file: " << encFile << std::endl;

    std::ifstream in_key(pvtFilename), in_msg(encFile);
    if (!in_key.is_open())
        std::cerr << "Error open Private Key file.\n";
    if (!in_msg.is_open())
        std::cerr << "Error open Cipher file.\n";

    std::string line;
    mpz_t pvtkey, n;
    mpz_inits(pvtkey, n, nullptr);

    while (getline(in_key, line))
    {
        if (line == "Private key:")
        {
            getline(in_key, line);
            mpz_set_str(pvtkey, line.c_str(), 16);
        }
        else if (line == "n:")
        {
            getline(in_key, line);
            mpz_set_str(n, line.c_str(), 16);
        }
    }

    // Read the message from file and convert it to an integer
    std::string msg;
    std::stringstream ss;

    // Đọc từng dòng trong tệp và nối vào stringstream
    while (getline(in_msg, line))
    {
        ss << line; // Lưu dữ liệu vào stringstreamg
    }

    // Chuyển đổi stringstream thành chuỗi
    msg = ss.str();

    mpz_t rop, x;
    mpz_inits(rop, x, nullptr);
    mpz_set_str(x, msg.c_str(), 10);

    power(rop, x, pvtkey, n);

    std::cout << "Decrypted message: ";
    mpz_out_str(stdout, 10, rop);
    std::cout << std::endl;

    // Convert decrypted result to string (plain text) and save in folder decrypted
    std::string decrypted_file = DECRYPTED_DIR + this->filePlaintext;
    writeFileFromGMPvariable(rop, decrypted_file);

    std::cout << "Plaintext: " << std::endl;
#if defined(_WIN32) || defined(_WIN64)
    std::string show_content_cmd = "type " + decrypted_file;
    system(show_content_cmd.c_str());
#elif defined(__linux__)
    std::string show_content_cmd = "cat " + decrypted_file;
    system(show_content_cmd.c_str());
#endif

    // STORE ROP TO .DEC FILE
    std::string result = mpz_get_str(nullptr, 10, rop);

    std::string decFile = encFile.replace(encFile.find('.'), 4, DECODE_EXT);
    std::ofstream out(decFile);
    if (out.is_open())
    {
        out << result;
    }

    out.close();
    in_key.close();
    in_msg.close();
    return;
}