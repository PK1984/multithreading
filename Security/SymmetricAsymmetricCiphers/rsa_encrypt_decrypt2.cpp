#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <iostream>
#include <string>


// Encrypt data using public key
std::string encrypt_rsa(const std::string& plain_text, EVP_PKEY* public_key) {
    RSA* rsa_key = EVP_PKEY_get1_RSA(public_key);
/*  DSA    *EVP_PKEY_get1_DSA    (EVP_PKEY *public_key); #Digital Signature Algorithm
	DH     *EVP_PKEY_get1_DH     (EVP_PKEY *public_key); # Diffie-Hellman
	EC_KEY *EVP_PKEY_get1_EC_KEY (EVP_PKEY *public_key); # Elliptic curve */

    int key_size = RSA_size(rsa_key);
    unsigned char* buffer = new unsigned char[key_size];
    int len = RSA_public_encrypt(plain_text.length(), (const unsigned char*)plain_text.c_str(),
                                  buffer, rsa_key, RSA_PKCS1_OAEP_PADDING);

    std::string cipher_text((const char*)buffer, len);
    delete[] buffer;
    RSA_free(rsa_key);
    return cipher_text;
}

// Decrypt data using private key
std::string decrypt_rsa(const std::string& cipher_text, EVP_PKEY* private_key) {
    RSA* rsa_key = EVP_PKEY_get1_RSA(private_key);

    int key_size = RSA_size(rsa_key);
    unsigned char* buffer = new unsigned char[key_size];
    int len = RSA_private_decrypt(cipher_text.length(), (const unsigned char*)cipher_text.c_str(),
                                   buffer, rsa_key, RSA_PKCS1_OAEP_PADDING);

    std::string plain_text((const char*)buffer, len);
    delete[] buffer;
    RSA_free(rsa_key);
    return plain_text;
}

// Example usage
int main() {
    // Load public key from file
    FILE* file = fopen("public_key.pem", "r");
    EVP_PKEY* public_key = PEM_read_PUBKEY(file, NULL, NULL, NULL);
    fclose(file);

    // Load private key from file
    file = fopen("private_key.pem", "r");
    EVP_PKEY* private_key = PEM_read_PrivateKey(file, NULL, NULL, NULL);
    fclose(file);

    // Encrypt plaintext using public key
    std::string plain_text = "Hello, world!";
    std::string cipher_text = encrypt_rsa(plain_text, public_key);

    // Decrypt ciphertext using private key
    std::string decrypted_text = decrypt_rsa(cipher_text, private_key);

    // Print original plaintext and decrypted plaintext
    std::cout << "Original text: " << plain_text << std::endl;
    std::cout << "Decrypted text: " << decrypted_text << std::endl;

    // Free memory
    EVP_PKEY_free(public_key);
    EVP_PKEY_free(private_key);

    return 0;
}
