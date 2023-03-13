#include <iostream>
#include <string>
#include <openssl/evp.h>
 
using namespace std;
 
// Function to encrypt the plaintext using a symmetric cipher
string encrypt_symmetric(const string& plain_text, const string& cipher_name, const string& key)
{
    const EVP_CIPHER* cipher = EVP_get_cipherbyname(cipher_name.c_str());
    if (!cipher) {
        cerr << "Invalid cipher name." << endl;
        exit(1);
    }
 
	// create new context
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
	std::cout << "Secret key has length of: " << key.length() << std::endl;
    EVP_EncryptInit_ex(ctx, cipher, NULL, (const unsigned char*)key.c_str(), NULL);
    // - We have updated the init function, what should we call it?
    // - Just add '2' at the end
    //~ if (!EVP_EncryptInit_ex2(ctx, EVP_idea_cbc(), key, iv, NULL)) {
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
    EVP_EncryptUpdate(ctx, cipher_text_buffer, &out_len, (const unsigned char*)plain_text.c_str(), plain_text.length());
    std::cout << "post EVP_EncryptUpdate out_len: " << out_len << std::endl;     
    
    // If padding is enabled (the default) then EVP_EncryptFinal_ex() encrypts the "final" data, 
    // that is any data that remains in a partial block. 
    // If padding is disabled then EVP_EncryptFinal_ex() will not encrypt any more data and it will 
    // return an error if any data remains in a partial block: that is if the total data length is not
    //  a multiple of the block size.
	int final_len = 0;
    EVP_EncryptFinal_ex(ctx, cipher_text_buffer + out_len, &final_len);
    std::cout << "EVP_EncryptFinal_ex final bytes written: " << final_len << std::endl; 
 
    string result((char*)cipher_text_buffer, out_len + final_len);
 
    delete[] cipher_text_buffer;
    EVP_CIPHER_CTX_free(ctx);
 
    return result;
}
 
// Function to decrypt the ciphertext using a symmetric cipher
string decrypt_symmetric(const string& cipher_text, const string& cipher_name, const string& key)
{
    const EVP_CIPHER* cipher = EVP_get_cipherbyname(cipher_name.c_str());
    if (!cipher) {
        cerr << "Invalid cipher name." << endl;
        exit(1);
    }
 
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    EVP_DecryptInit_ex(ctx, cipher, NULL, (const unsigned char*)key.c_str(), NULL);
 
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
    string key = "mysecretkey";
 
    string cipher_text = encrypt_symmetric(plain_text, cipher_name, key);
    cout << "Cipher text: " << cipher_text << endl;
 
    string decrypted_text = decrypt_symmetric(cipher_text, cipher_name, key);
    cout << "Decrypted text: " << decrypted_text << endl;
 
    return 0;
}
