#include <iostream>
#include <string>
#include <openssl/evp.h>
#include <openssl/rand.h>


using namespace std;

// Function to encrypt the plaintext using a symmetric cipher
string encrypt_symmetric(const string& plain_text, const string& cipher_name, const string& key, const string& initialization_vector)
{
    std::cout << "Secret key has length of: " << key.length() << std::endl;
    std::cout << "Initialization vector has length of: " << initialization_vector.length() << std::endl;

    const EVP_CIPHER* cipher = EVP_get_cipherbyname(cipher_name.c_str());
    if (!cipher) {
        cerr << "Invalid cipher name." << endl;
        exit(1);
    }

    // create new context
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();

    if (!EVP_EncryptInit_ex(
        ctx,                                                    // context
        cipher,                                                 // cipher type
        NULL,                                                   // engine implementation from the cipher will be used
        (const unsigned char*)key.c_str(),                      // the actual length of the symmetric key depends on the cipher
        (const unsigned char*)initialization_vector.c_str()))   // for some ciphers the initialization vector may be generated internally if not specified, for other ciphers it may not be needed altogether
    {
        EVP_CIPHER_CTX_free(ctx);
        exit(1);
    }
    //~ if (!EVP_EncryptInit_ex2(ctx, cipher, key, iv, OSSL_PARAM)) {
        //~ /* Error */
        //~ EVP_CIPHER_CTX_free(ctx);
        //~ return 0;
    //~ }

    int out_len = plain_text.length() + EVP_CIPHER_block_size(cipher);
    std::cout << "plain_text.length(): " << plain_text.length() << std::endl;
    std::cout << "EVP_CIPHER_block_size("<<cipher_name<<"): " << EVP_CIPHER_block_size(cipher) << std::endl;

    unsigned char* cipher_text_buffer = new unsigned char[out_len];
    // Encrypts plain_text.length() bytes from the buffer plain_text.c_str() and writes to cipher_text_buffer
    // Can be called multiple times to encrypt successive blocks of data.
    // The actual number of bytes written is placed in out_len
    std::cout << "pre EVP_EncryptUpdate out_len: " << out_len << std::endl;
    if (!EVP_EncryptUpdate(
            ctx,
            cipher_text_buffer,
            &out_len,
            (const unsigned char*)plain_text.c_str(),
            plain_text.length()))
    {
        EVP_CIPHER_CTX_free(ctx);
        delete[] cipher_text_buffer;
        exit(1);
    }
    std::cout << "post EVP_EncryptUpdate out_len: " << out_len << std::endl;

    // If padding is enabled (the default) then EVP_EncryptFinal_ex() encrypts the "final" data,
    // that is any data that remains in a partial block.
    // If padding is disabled then EVP_EncryptFinal_ex() will not encrypt any more data and it will
    // return an error if any data remains in a partial block: that is if the total data length is not
    //  a multiple of the block size.
    int final_len = 0;
    if (!EVP_EncryptFinal_ex(ctx, cipher_text_buffer + out_len, &final_len))
    {
        EVP_CIPHER_CTX_free(ctx);
        delete[] cipher_text_buffer;
        exit(1);
    }

    std::cout << "EVP_EncryptFinal_ex final bytes written: " << final_len << std::endl;

    string result((char*)cipher_text_buffer, out_len + final_len);

    delete[] cipher_text_buffer;
    EVP_CIPHER_CTX_free(ctx);

    return result;
}

// Function to decrypt the ciphertext using a symmetric cipher
string decrypt_symmetric(const string& cipher_text, const string& cipher_name, const string& key, const string& initialization_vector)
{
    const EVP_CIPHER* cipher = EVP_get_cipherbyname(cipher_name.c_str());
    if (!cipher) {
        cerr << "Invalid cipher name." << endl;
        exit(1);
    }

    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    EVP_DecryptInit_ex(ctx, cipher, NULL, (const unsigned char*)key.c_str(), (const unsigned char*)initialization_vector.c_str());

    int out_len = cipher_text.length();
    unsigned char* plain_text = new unsigned char[out_len];
    int final_len = 0;

    EVP_DecryptUpdate(ctx, plain_text, &out_len, (const unsigned char*)cipher_text.c_str(), cipher_text.length());
    EVP_DecryptFinal_ex(ctx, plain_text + out_len, &final_len);

    string result((char*)plain_text, out_len + final_len);

    delete[] plain_text;
    EVP_CIPHER_CTX_free(ctx);

    return result;
}

// Driver code to test the encryption and decryption functions
int main()
{
    string plain_text = "Hello, world!";
    string cipher_name = "AES-128-CBC";
    string key = "mysecretkey_____"; // secret key should have 16 bytes for AES-128!
    string initialization_vector = "random init vect"; // initialization vector should be at least block size (128bits for aes)
    string cipher_text = encrypt_symmetric(plain_text, cipher_name, key, initialization_vector);
    cout << "Cipher text len: " << cipher_text.length() << endl;

    string decrypted_text = decrypt_symmetric(cipher_text, cipher_name, key, initialization_vector);
    cout << "Decrypted text: " << decrypted_text << endl;

    return 0;
}
